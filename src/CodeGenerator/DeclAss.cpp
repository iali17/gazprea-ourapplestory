//
// Created by ali5 on 11/20/18.
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
extern llvm::Type *streamStateTy;

/**
 * Creates declarations statements and adds the
 * new variables into the symbolTable
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(DeclNode *node) {
    llvm::Value *val = visit(node->getExpr());
    llvm::Value *ptr = nullptr;

    if (node->getTypeIds()->empty() && ((node->getType() == TUPLE) || it->isVectorType(val))) {
        ptr = val;
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    } else if (node->getTypeIds()->empty() && it->isTupleType(val)) {
        ptr = ir->CreateAlloca(val->getType()->getPointerElementType());
        ptr = it->initTuple(ptr, it->getValueVectorFromTuple(val));
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), ptr);
    } else if (node->getTypeIds()->empty()) {
        ptr = ir->CreateAlloca(val->getType());
        ir->CreateStore(val, ptr);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    } else if (node->getTypeIds()->size() == 1) {
        llvm::Type *type = symbolTable->resolveType(node->getTypeIds()->at(0))->getTypeDef();
        node->setLlvmType(type);

        ptr = ir->CreateAlloca(type);
        if(val == nullptr) {
            if (!(it->setNull(type, ptr))) {
                std::cerr << "Unable to initialize to null at line " << node->getLine() << ". Aborting...\n";
                exit(0);
            }
            symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), ptr);
            return nullptr;
        }

        if (type->isStructTy()) {
            ptr = it->initTuple(ptr, it->getValueVectorFromTuple(val));
        } else if (type->isVectorTy()) {
            ptr = ct->typeAssCast(ptr->getType(), val, node->getLine());
        } else {
            val = ct->typeAssCast(type, val, node->getLine());
            ir->CreateStore(val, ptr);
        }

        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    }

    symbolTable->resolveSymbol(node->getID())->setPtr(ptr);
    //TODO - ALL OTHER CASES
    return nullptr;
}

/**
 *
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(StreamDeclNode *node) {
    llvm::Value *ptr = ir->CreateAlloca(streamStateTy);

    llvm::Value *structElemPtr = ir->CreateInBoundsGEP(ptr,{it->getConsi32(0), it->getConsi32(0)});
    ir->CreateStore(it->getConsi32(node->getType()), structElemPtr);
    structElemPtr = ir->CreateInBoundsGEP(ptr,{it->getConsi32(0), it->getConsi32(1)});
    ir->CreateStore(it->getConsi32(-1), structElemPtr);

    symbolTable->addSymbol(node->getId(), node->getType(), false, ptr);
    return nullptr;
}


/**
 * Deals with global constants
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(GlobalDeclNode *node) {
    llvm::Value *val = visit(node->getExpr());

    //set constant
    auto *cons = llvm::cast<llvm::Constant>(val);//llvm::ConstantInt::get(intTy, 0, true);
    auto *consLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(node->getID(), cons->getType())
            );
    consLoc->setInitializer(cons);

    //send signal eleven if anyone tries to assign to it
    symbolTable->addSymbol(node->getID(), UNDEF, true, nullptr);
    return nullptr;
}

/**
 * Deals with assignments
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(AssignNode *node) {
    Symbol *left, *right;
    left = symbolTable->resolveSymbol(node->getID());
    assert(!left->isConstant());

    if (dynamic_cast<IDNode *>(node->getExpr())) {
        auto idNode = (IDNode *) node->getExpr();
        right = symbolTable->resolveSymbol(idNode->getID());
        std::string outString = "std_output()";
        std::string inString = "std_input()";

        if (((left->getType() == INSTREAM) || (left->getType() == OUTSTREAM)) &&
            left->getType() != right->getType()) {

            auto *error = new ScalarErrorNode(outString, inString, node->getLine());
            eb ->printError(error);
        } else if (((left->getType() == INSTREAM) || (left->getType() == OUTSTREAM)) &&
                   left->getType() == right->getType()) {
            return nullptr;
        }
    }

    llvm::Value *val = visit(node->getExpr());
    llvm::Value *ptr = left->getPtr();

    if (it->isVectorType(left->getPtr())) {
        if (dynamic_cast<IdnNode *>(node->getExpr())) {
            et->setIdentityVector(ptr);
            return nullptr;
        } else if (dynamic_cast<NullNode *>(node->getExpr())) {
            et->setNullVector(ptr);
            return nullptr;
        } else {
            ptr = ct->typeAssCast(ptr->getType(), val, node->getLine(),
                                  nullptr, (int) ((VectorNode *) node->getExpr())->getElements()->size());
            left->setPtr(ptr);
        }
    }

    if (val) {
        if (it->isTupleType(left->getPtr())) {
            ptr = it->initTuple(ptr, it->getValueVectorFromTuple(val));
            left->setPtr(ptr);
        } else {
            val = ct->typeAssCast(ptr->getType()->getPointerElementType(), val, node->getLine());
            ir->CreateStore(val, ptr);
        }

    } else if (!(it->setNull(ptr->getType()->getPointerElementType(), ptr))){
        std::cerr << "Unable to initialize to null at line " << node->getLine() <<". Aborting...\n";
    }
    return nullptr;
}

/**
 * deals with slice assignment. we extract some data and then pass it to the appropraite function
 * @param node
 * @return
 */
llvm::Value *CodeGenerator::visit(SliceAssignNode *node) {
    llvm::Value *dest                = visit(((IndexNode *)node->getLeft())->getLHS());
    std::vector<ASTNode *> *idxNodes = ((IndexNode *)node->getLeft())->getIndexExpr();
    llvm::Value *src                 = visit(node->getRight());

    auto *idxVec = new std::vector<llvm::Value *>;
    for(uint i = 0; i < idxNodes->size(); i++){
        idxVec->push_back(visit(idxNodes->at(i)));
    }

    //vector only
    if(it->isVectorType(dest))
        return vectorSliceAssign(node->getRight(), (IndexNode *) node->getLeft(), dest, idxVec, src);

    //matrix
    if(it->isMatrixType(dest))
        return matrixSliceAssign(node->getRight(), (IndexNode *) node->getLeft(), dest, idxVec, src);

    return nullptr;
}