//
// Created by ali5 on 11/21/18.
//

#include <CodeGenerator/CodeGenerator.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
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
 * Deals with implicit and explicit casting.
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(CastExprNode *node) {
    if(node->getCastType() == CastExprNode::TUPLE_CAST) {
        return tupleCasting(node);
    } else if(node->getCastType() == CastExprNode::VECTOR_CAST) {
        return vectorCasting(node);
    } else if(node->getCastType() == CastExprNode::MATRIX_CAST) {
        return matrixCasting(node);
    } else if(node->getCastType() == CastExprNode::SCALAR_CAST) {
        return scalarCasting(node);
    } else {
        return nullptr;
    }
}

/**
 * Deals with tuple casting
 *
 * @param node
 * @return
 */
llvm::Value *CodeGenerator::tupleCasting(CastExprNode *node) {
    llvm::Type *type;
    llvm::Value *expr;

    auto *values = new std::vector<llvm::Value *>();
    llvm::StructType *types = parseStructType(dynamic_cast<TupleType *>(dynamic_cast<TupleCastNode *>(node)->getTuple()));
    llvm::Value *exprP = visit(node->getExpr());
    llvm::Value *ptr = ir->CreateAlloca(types);


    // Checks if casting another type to a tuple
    if(!it->isStructType(exprP)) {
        std::string left = "tuple(*)";
        std::string right = it->getType(exprP->getType(), exprP);
        auto *error = new ScalarNode(left, right, node->getLine()); // print error message and abort
        eb->printError(error);
    }

    // Checks if tuples are different size
    if(types->getStructNumElements() != exprP->getType()->getPointerElementType()->getStructNumElements()){
        std::cerr << "Mismatching tuple lengths on line " << node->getLine() <<". Aborting...\n";
        exit(1);
    }

    for(unsigned long i = 0; i < types->elements().size(); ++i) {
        expr = it->getValFromTuple(exprP, it->getConsi32(i));
        type = types->elements()[i];

        expr = ct->varCast(type->getPointerElementType(), expr, node->getLine());
        values->push_back(expr);
    }
    return it->initTuple(ptr, values);
}

/**
 * Deals with vector casting
 *
 * @param node
 * @return
 */
llvm::Value* CodeGenerator::vectorCasting(CastExprNode *node) {
    std::cout << "THIS IS VECTOR CASTING" << std::endl;

    auto values = new std::vector<llvm::Value *>();
    llvm::Type *type;
    llvm::Value *expr;
    llvm::Value *exprP = visit(node->getExpr());

    ASTNode *vnode = dynamic_cast<VectorCastNode *>(node)->getVector();
    std::string stype = dynamic_cast<VectorType *>(vnode)->getStringType();
    type = it->getVectorType(stype);

    // Extension size if exists
    llvm::Value *size  = visit(dynamic_cast<VectorType *>(vnode)->getSize());

    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));
    vec = it->castVectorToType(vec, type);

    // This is for cases such as: as<integer vector[3]>(1)
    if(!it->isStructType(exprP)) {
        // Todo: Add error for casting without extension (size)
        if(!size) {
            assert(size);
        }

        et->initVector(vec, size);
        vec = it->castVectorToType(vec, type);

        for(unsigned long i = 0; i < llvm::dyn_cast<llvm::ConstantInt>(size)->getSExtValue(); i++) {
            expr = ct->varCast(type, exprP, node->getLine());
            values->push_back(expr);
        }

        it->setVectorValues(vec, values);
        return vec;
    }

    else if(exprP->getType() == intervalTy) {
        std::cout << "this is interval to vector casting\n";

        return nullptr;
    }

    else {
        vec = vectorNoSizeCast(vec, exprP, type, node->getLine());

        if(size) {
            llvm::Value *newVec = et->getNewVector(it->getConstFromType(type));
            newVec = it->castVectorToType(newVec, type);
            et->initVector(newVec, size);

            et->copyVectorElements(newVec, vec);

            return newVec;
        }

        return vec;
    }

}

/**
 * Deals with matrix casting
 *
 * @param node
 * @return
 */
llvm::Value* CodeGenerator::matrixCasting(CastExprNode *node) {
    std::cout << "THIS IS MATRIX CASTING\n";

    return nullptr;
}

/**
 *
 * @param node
 * @return
 */
llvm::Value* CodeGenerator::scalarCasting(CastExprNode *node) {
    llvm::Type *type;
    llvm::Value *expr;
    llvm::Value *exprP = visit(node->getExpr());

    // Casting error when casting tuple to scalar
    if(it->isStructType(exprP)) {
        std::string left = dynamic_cast<ScalarCastNode *>(node)->getTypeString();
        std::string right = "tuple(*)";
        auto *error = new ScalarNode(left, right, node->getLine()); // print error message and abort
        eb->printError(error);
    }

    expr = visit(node->getExpr());
    GazpreaType *gazpreaType = symbolTable->resolveType(dynamic_cast<ScalarCastNode *>(node)->getTypeString());
    type = gazpreaType->getTypeDef();

    return ct->varCast(type, expr, node->getLine());
}

llvm::Value *CodeGenerator::vectorNoSizeCast(llvm::Value *vec, llvm::Value *exprP, llvm::Type *type, int line) {
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    llvm::Value *elemPtr;
    llvm::Value *elemValue;
    llvm::Value *newVecElemPtr;
    llvm::Value *elements = it->getPtrFromStruct(exprP, it->getConsi32(VEC_ELEM_INDEX));

    // Counter
    llvm::Value *curVecSize = it->getConsi32(0);
    llvm::Value *curVecPtr = ir->CreateAlloca(intTy);

    ir->CreateStore(curVecSize, curVecPtr);

    // Max loop count
    llvm::Value *maxSize = it->getValFromStruct(exprP, it->getConsi32(VEC_LEN_INDEX));

    // Creates empty vector of cast type and size of expr
    et->initVector(vec, maxSize);

    wb->beginWhile();

    curVecSize = ir->CreateLoad(curVecPtr);
    llvm::Value *cmpStatement = ir->CreateICmpSLT(curVecSize, maxSize, "CompStatement");

    // Body
    wb->insertControl(cmpStatement);

    // This loads a pointer to the position in the vector
    elemPtr = ir->CreateGEP(elements, curVecSize);
    elemValue = ir->CreateLoad(elemPtr);

    // Casted element value
    elemValue = ct->varCast(type, elemValue, line);

    newVecElemPtr = it->getPtrFromStruct(vec, it->getConsi32(VEC_ELEM_INDEX));
    newVecElemPtr = ir->CreateGEP(newVecElemPtr, curVecSize);

    // Store casted value into new vector at current position
    ir->CreateStore(elemValue, newVecElemPtr);

    // Increment counter
    curVecSize = ir->CreateAdd(curVecSize, it->getConsi32(1));
    ir->CreateStore(curVecSize, curVecPtr);

    wb->endWhile();

    return vec;
}
