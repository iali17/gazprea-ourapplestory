//
// Created by ali5 on 11/20/18.
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

    if(val && it->isVectorType(val)){
        val = et->getVectorCopy(val);

        if(val && (val->getType() == strTy->getPointerTo())) {
            val = ir->CreatePointerCast(val, strTy->getPointerTo());
        }
    }
    else if(val && it->isIntervalType(val)){
        val = et->getNewInterval(it->getValFromStruct(val, INTERVAL_MIN), it->getValFromStruct(val, INTERVAL_MAX));
    }
    else if(val && it->isMatrixType(val)){
        val = et->getMatrixCopy(val);
    }


    if (node->getTypeIds()->empty() && (node->getType() == TUPLE || it->isIntervalType(val) || it->isMatrixType(val) || it->isVectorType(val))){
        ptr = val;
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    } else if (node->getTypeIds()->empty() && it->isTupleType(val)) {
        ptr = ir->CreateAlloca(val->getType()->getPointerElementType());
        ptr = initTuple(ptr, it->getValueVectorFromTuple(val));
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), ptr);
    } else if (node->getTypeIds()->empty()) {
        ptr = ir->CreateAlloca(val->getType());
        ir->CreateStore(val, ptr);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    } else if (node->getTypeIds()->size() == 1) {
        GazpreaType *gazType = symbolTable->resolveType(node->getTypeIds()->at(0));
        bool garboCond = false;

        llvm::Type *type = gazType->getTypeDef();

        node->setLlvmType(type);
        ptr = ir->CreateAlloca(type);

        // User defined matrix type
        if (it->isMatrixType(ptr)) {
            garboCond = true;
            llvm::Value *typeConst = it->getConstFromType(type);
            ptr = et->getNewMatrix(typeConst);
            et->initMatrix(ptr, it->getConsi32(gazType->getDim1()), it->getConsi32(gazType->getDim2()));
            ptr = it->castMatrixToType(ptr, type->getPointerTo());
        }
        // User defined vector type
        else if (it->isVectorType(ptr)) {
            garboCond = true;
            llvm::Value *typeConst = it->getConstFromType(type);
            ptr = et->getNewVector(typeConst);
            et->initVector(ptr, it->getConsi32(gazType->getDim1()));
            ptr = it->castVectorToType(ptr, type->getPointerTo());
        }
        // User defined interval type
        else if (it->isIntervalType(ptr)) {
            garboCond = true;
            ptr = et->getNewInterval(it->getConsi32(0), it->getConsi32(0));
        }

        if (val == nullptr && !garboCond) {
            if (!(it->setNull(type, ptr))) {
                std::cerr << "Unable to initialize to null at line " << node->getLine() << ". Aborting...\n";
                exit(0);
            }
            symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), ptr);
            return nullptr;
        }

        if (it->isTupleType(ptr)) {
            ptr = initTuple(ptr, it->getValueVectorFromTuple(val));
        } else if (it->isVectorType(ptr)) {
            llvm::Value *tempVal = ct->typeAssCast(gazType->getTypeDef(), val, node->getLine(), it->getConsi32(gazType->getDim1()));
            et->strictCopyVectorElements(ptr, tempVal, it->getConsi32(node->getLine()), it->getConsi32(true));
        } else if (it->isMatrixType(ptr)) {
            llvm::Value *tempVal =  ct->typeAssCast(gazType->getTypeDef(), val, node->getLine(), it->getConsi32(gazType->getDim1()), it->getConsi32(gazType->getDim2()));
            et->strictCopyMatrixElements(ptr, tempVal, it->getConsi32(node->getLine()), it->getConsi32(true));
        } else if (it->isIntervalType(ptr)) {
            llvm::Value *lowerPtr = it->getPtrFromStruct(ptr, INTERVAL_MIN);
            llvm::Value *upperPtr = it->getPtrFromStruct(ptr, INTERVAL_MAX);

            llvm::Value *newLower = it->getValFromStruct(val, INTERVAL_MIN);
            llvm::Value *newUpper = it->getValFromStruct(val, INTERVAL_MAX);

            ir->CreateStore(newLower, lowerPtr);
            ir->CreateStore(newUpper, upperPtr);
        } else if(not(garboCond ) ){
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
            llvm::Value *l_ptr = left->getPtr();
            llvm::Value *r_ptr = right->getPtr();
            assert(l_ptr->getType() == r_ptr->getType());
            assert(l_ptr->getType()->getPointerElementType()->isStructTy());
            llvm::Value * from = ir->CreateGEP(r_ptr, {it->getConsi32(0), it->getConsi32(STREAM_STATE_INDEX)});
            llvm::Value * to   = ir->CreateGEP(l_ptr, {it->getConsi32(0), it->getConsi32(STREAM_STATE_INDEX)});
            ir->CreateStore(ir->CreateLoad(from), to);

            return nullptr;
        }
    }

    llvm::Value *val;// = visit(node->getExpr());
    llvm::Value *ptr = left->getPtr();

    if (it->isVectorType(left->getPtr())) {
        val = visit(node->getExpr());
        if (dynamic_cast<IdnNode *>(node->getExpr())) {
            et->setIdentityVector(ptr);
        } else if (dynamic_cast<NullNode *>(node->getExpr())) {
            et->setNullVector(ptr);
        } else {
            et->strictCopyVectorElements(ptr, val, it->getConsi32(node->getLine()), it->getConsi32(false));
        }

        if(val && (val->getType() == strTy->getPointerTo() || ptr->getType() == strTy->getPointerTo())) {
            ptr = ir->CreatePointerCast(ptr, strTy->getPointerTo());
            left->setPtr(ptr);
        }
        return nullptr;
    }

    if (it->isIntervalType(ptr)) {
        val = visit(node->getExpr());
        if (dynamic_cast<NullNode *>(node->getExpr())){
            left->setPtr(et->getNewInterval(it->getConsi32(0), it->getConsi32(0)));
        }
        else if (dynamic_cast<IdnNode *>(node->getExpr())) {
            left->setPtr(et->getNewInterval(it->getConsi32(1), it->getConsi32(1)));
        }
        else {
            if (!it->isIntervalType(val)){ // todo get the correct right type toooo
                std::cerr << "Type error: Cannot convert between integer interval and * on line "
                << node->getLine() << ". Aborting...\n";
                exit(1);
            }

            llvm::Value * lowerPtr = it->getPtrFromStruct(ptr, INTERVAL_MIN);
            llvm::Value * upperPtr = it->getPtrFromStruct(ptr, INTERVAL_MAX);

            llvm::Value * newLower = it->getValFromStruct(val, INTERVAL_MIN);
            llvm::Value * newUpper = it->getValFromStruct(val, INTERVAL_MAX);

            ir->CreateStore(newLower, lowerPtr);
            ir->CreateStore(newUpper, upperPtr);
        }
        return nullptr;
    }

    if (it->isMatrixType(left->getPtr())) {
        val = visit(node->getExpr());
        if (dynamic_cast<IdnNode *>(node->getExpr())) {
            et->setIdentityMatrix(ptr);
        } else if (dynamic_cast<NullNode *>(node->getExpr())) {
            et->setNullMatrix(ptr);
        } else {
            et->strictCopyMatrixElements(ptr, val, it->getConsi32(node->getLine()), it->getConsi32(false));
            return nullptr;
        }
        return nullptr;
    }

    bool isTuple = it->isTupleType(left->getPtr());
    bool isIdn   = dynamic_cast<IdnNode *>(node->getExpr());
    bool isNull  = dynamic_cast<NullNode *>(node->getExpr());

    if ((isTuple && isNull) || val) {
        if(isTuple && isIdn){
            ptr = initTuple(IDENTITY, llvm::cast<llvm::StructType>(ptr->getType()->getPointerElementType()));
            left->setPtr(ptr);
        }
        else if(isTuple && isNull){
            ptr = initTuple(NULLTY, llvm::cast<llvm::StructType>(ptr->getType()->getPointerElementType()));
            left->setPtr(ptr);
        }
        else if (isTuple) {
            if(dynamic_cast<TupleNode *>(node->getExpr()))
                ptr = visit((TupleNode *) node->getExpr(), llvm::cast<llvm::StructType>(left->getPtr()->getType()->getPointerElementType()));
            else
                ptr = initTuple(ptr, it->getValueVectorFromTuple(visit(node->getExpr())));
            left->setPtr(ptr);
        } else {
            val = visit(node->getExpr());
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
    for (auto &idxNode : *idxNodes) {
        idxVec->push_back(visit(idxNode));
    }

    //vector only
    if(it->isVectorType(dest))
        return vectorSliceAssign(node->getRight(), (IndexNode *) node->getLeft(), dest, idxVec, src);

    //matrix
    if(it->isMatrixType(dest))
        return matrixSliceAssign(node->getRight(), (IndexNode *) node->getLeft(), dest, idxVec, src);

    return nullptr;
}