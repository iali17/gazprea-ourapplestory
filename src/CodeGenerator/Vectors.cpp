//
// Created by kyle on 20/11/18.
//

#include <CodeGenerator/CodeGenerator.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *strTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;
extern llvm::Type *vecTy;
extern llvm::Type *matrixTy;
extern llvm::Type *intVecTy;
extern llvm::Type *intMatrixTy;
extern llvm::Type *charVecTy;
extern llvm::Type *charMatrixTy;
extern llvm::Type *boolVecTy;
extern llvm::Type *boolMatrixTy;
extern llvm::Type *realVecTy;
extern llvm::Type *realMatrixTy;
extern llvm::Type *intervalTy;

/**
 * Handles vector literals
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(VectorNode *node) {
    auto tempValues = new std::vector<llvm::Value *>();
    auto values = new std::vector<llvm::Value *>();
    auto valueTypes = new std::vector<llvm::Type *>();

    // Get tempValues, check if need to upcast
    for (auto &i : *node->getElements()) {
        valueTypes->push_back(visit(i)->getType());
        tempValues->push_back(visit(i));
    }

    // Casting empty vector with proper type
    llvm::Type *type;
    if(!tempValues->empty()) type = it->getInitVectorType(*valueTypes);
    else type = intTy;

    // Cast elements to proper type
    for(auto vecElem : *tempValues) {
        values->push_back(ct->varCast(type, vecElem, node->getLine()));
    }

    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));
    et->initVector(vec, it->getConsi32(values->size()));

    vec = it->castVectorToType(vec, type);

    // Populate vector
    if(!values->empty())
        it->setVectorValues(vec, values);

    return vec;
}

/**
 * Handles string literals
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(StringNode *node) {
    auto values = new std::vector<llvm::Value *>();

    for (auto &i : *node->getElements())
        values->push_back(visit(i));

    // Casting to a stringTy
    llvm::Value *vec = et->getNewVector(it->getConstFromType(charTy));
    et->initVector(vec, it->getConsi32(values->size()));

    vec = it->castVectorToType(vec, charTy);
    vec = ir->CreatePointerCast(vec, strTy->getPointerTo());
    // Populate vector
    if(!values->empty())
        it->setVectorValues(vec, values);

    return vec;
}

/**
 * Handles vector declarations
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(VectorDeclNode *node) {
    VectorNode *vectorNode = nullptr;
    StringNode *stringNode = nullptr;
    auto *vectorType = dynamic_cast<VectorTypeNode *>(node->getVectorType());

    vectorNode = dynamic_cast<VectorNode *>(node->getExpr());
    stringNode = dynamic_cast<StringNode *>(node->getExpr());

    std::string stype = vectorType->getStringType();
    llvm::Type *type = it->getVectorType(stype);
    llvm::Value *size = visit(node->getSize());

    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));
    vec = it->castVectorToType(vec, type);

    if (size){
        et->initVector(vec, size);
    }

    if (dynamic_cast<IdnNode *>(node->getExpr())) {
        et->setIdentityVector(vec);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), vec);
        return nullptr;
    } else if (dynamic_cast<NullNode *>(node->getExpr())) {
        et->setNullVector(vec);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), vec);
        return nullptr;
    }

    if (vectorNode || stringNode) {
        llvm::Value *vecExpr = visit(node->getExpr());
        llvm::Type *vecType = it->getDeclVectorType(stype);

        int exprSize;
        if (vectorNode) {
            exprSize = (int)vectorNode->getElements()->size();
        } else {
            exprSize = (int) stringNode->getElements()->size();
        }

        // they have declared an empty vector
        if ((vecExpr->getType() == intVecTy->getPointerTo() || vecExpr->getType() == intVecTy) && (type!= intTy && type != realTy)) {
            vecExpr = it->castVectorToType(vecExpr, type);
        }

        vec = ct->typeAssCast(vecType, vecExpr, node->getLine(), size, nullptr, exprSize);

        if(stringNode) {
            vec = ir->CreatePointerCast(vec, strTy->getPointerTo());
        }

    }

    // Handles case when expr is not a vector -> scalar, haven't tested tuple. Pretty sure segfaults if tested
    else if (node->getExpr()) {
        llvm::Value *regExpr = visit(node->getExpr());
        llvm::Type *vecType = it->getDeclVectorType(stype);

        vec = ct->typeAssCast(vecType, regExpr, node->getLine(), size);

        if(stype == "string") {
            vec = ir->CreatePointerCast(vec, strTy->getPointerTo());
        }
    }

    symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), vec);
    return nullptr;
}

/**
 * Reverse vector built in function
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(ReverseVectorNode *node) {
    llvm::Value *vecPtr = visit(node->getExpr());
    return et->getReverseVector(vecPtr);
}

/**
 * Handles vector or matrix indexing
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(IndexNode *node) {
    llvm::Value *LHS = visit(node->getLHS());

    // indexing a vector
    if ((node->getIndexExpr()->size() == 1) && (it->isVectorType(LHS))) {
        llvm::Value *idx = visit(node->getIndexExpr()->at(0));
        return indexVector(LHS, idx, node->isSlice());
    }
    // indexing a matrix
    else if ((node->getIndexExpr()->size() == 2) && (it->isMatrixType(LHS))) {
        llvm::Value *rowIdx = visit(node->getIndexExpr()->at(0));
        llvm::Value *colIdx = visit(node->getIndexExpr()->at(1));
        return indexMatrix(LHS, rowIdx, colIdx, node->isSlice());
    }
    return nullptr;
}

/**
 * Length of vector built in function
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(LengthNode *node) {
    llvm::Value *vec = visit(node->getExpr());
    return et->getVectorLength(vec);
}

/**
 * Handles indexing vectors
 *
 * @param vec
 * @param idx
 * @param isSlice
 * @return
 */
llvm::Value *CodeGenerator::indexVector(llvm::Value *vec, llvm::Value *idx, bool isSlice) {
    //cover interval case
    if(it->isIntervalType(idx))
        idx = et->getVectorFromInterval(idx, it->getConsi32(1));

    //general
    if(it->isVectorType(idx))
        return et->getVectorSlice(vec, idx);

    llvm::Value *ptr = et->getVectorElementPointer(vec, idx);
    llvm::Type  *ty  = it->getVectorElementType(vec);
    ptr = ir->CreatePointerCast(ptr, ty->getPointerTo());

    if(isSlice)
        return ptr;
    return ir->CreateLoad(ptr);
}

/**
 * There are teo main cases:
 *  - index by integer
 *  - index by vector
 * In the first case we can do the store directly, otherwise we call an external function to do the assignment
 * @param idxExpr
 * @param dest
 * @param idxVec
 * @param src
 * @return
 */
llvm::Value *CodeGenerator::vectorSliceAssign(ASTNode * srcNode, IndexNode * idxExpr, llvm::Value *dest, std::vector<llvm::Value *> * idxVec, llvm::Value *src){
    llvm::Value *idx     = idxVec->at(0);
    ASTNode     *idxNode = idxExpr->getIndexExpr()->at(0);

    //check if it is an index assign
    llvm::Value * sad = visit(idxExpr->getIndexExpr()->at(0));

    //handle index assignment
    if(not(sad->getType()->isPointerTy())) {
        return indexAssign(srcNode, idxExpr, src, dest, idxExpr->getLine());
    }

    //modify vector on build in cases
    if (dynamic_cast<IdnNode *>(srcNode)){
        src = visit(idxNode);
        et->setNullVector(src);
    }
    else if (dynamic_cast<NullNode *>(srcNode)){
        src = visit(idxNode);
        et->setIdentityVector(src);
    }

    llvm::Value * size = it->getValFromStruct(idx, VEC_LEN_INDEX);
    src = ct->typeAssCast(dest->getType()->getPointerElementType(), src, srcNode->getLine(), size);

    et->assignFromVector(dest, idx, src);
    return nullptr;
}

/**
 * Handles vector index assignments
 *
 * @param srcNode
 * @param idxExpr
 * @param src
 * @param dest
 * @param line
 * @return nullptr
 */
llvm::Value *CodeGenerator::indexAssign(ASTNode *srcNode, IndexNode *idxExpr, llvm::Value *src, llvm::Value *dest,
                                        int line){
    if (dest->getType() == realVecTy->getPointerTo() && src->getType() == intTy)
        src = ct->varCast(realTy, src, line);

    llvm::Value * sad   = visit(idxExpr);
    llvm::Type  *destTy = sad->getType()->getPointerElementType();

    //garbage
    if (dynamic_cast<IdnNode *>(srcNode))
        ir->CreateStore(it->getIdn(destTy), sad);
    else if (dynamic_cast<NullNode *>(srcNode))
        ir->CreateStore(it->getNull(destTy), sad);
    else
        src = ct->typeAssCast(destTy, src, srcNode->getLine());
        ir->CreateStore(src, sad);
    return nullptr;
}