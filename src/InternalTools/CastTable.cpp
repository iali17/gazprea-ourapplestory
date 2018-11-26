//
// Created by ali5 on 10/26/18.
//
#include <InternalTools/CastTable.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;
extern llvm::Type *strTy;
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

CastTable::CastTable(llvm::LLVMContext *globalctx, llvm::IRBuilder<> *ir, InternalTools *it, ExternalTools *et, llvm::Module *mod, ErrorBuilder *eb) : globalCtx(globalctx), ir(ir), it(it), et(et), mod(mod), eb(eb) {
    // Do nothing
}

int CastTable::getType(llvm::Type *expr) {
    if(expr == i64Ty || expr == i32Ty || expr == intTy)
        return 2;
    else if(expr == realTy)
        return 3;
    else if(expr == boolTy)
        return 0;
    else if(expr == charTy || i8Ty)
        return 1;
    else if(expr == intervalTy)
        return 4;
    else if(expr == vecTy)
        return 5;
    else if(expr == matrixTy)
        return 6;
    return -1;
}

// This function is to check cast promotion without the use of the keyword: as
InternalTools::pair CastTable::typePromotion(llvm::Value *lValueLoad, llvm::Value *rValueLoad, int line) {
    assert(!((lValueLoad == nullptr) && (rValueLoad == nullptr)));

    if(!lValueLoad)
        lValueLoad = it->getNull(rValueLoad->getType());
    else if(!rValueLoad)
        rValueLoad = it->getNull(lValueLoad->getType());

    if(lValueLoad->getName() == "IdnNode")
        lValueLoad = it->getIdn(rValueLoad->getType());
    else if(rValueLoad->getName() == "IdnNode")
        rValueLoad = it->getIdn(lValueLoad->getType());

    if(it->isVectorType(lValueLoad) || it->isVectorType(rValueLoad)) {
        return vectorTypePromotion(lValueLoad, rValueLoad, line);
    }

    // Gazprea type of left and right expr
    llvm::Type *lTypeP = lValueLoad->getType();
    llvm::Type *rTypeP = rValueLoad->getType();

    // Position in typeTable
    int lType = getType(lTypeP);
    int rType = getType(rTypeP);

    // Cast type
    std::string castType = typePTable[lType][rType];
    std::string lTypeString = typePTable[lType][lType];
    std::string rTypeString = typePTable[rType][rType];

    if(castType == lTypeString && castType == rTypeString) {
        return it->makePair(lValueLoad, rValueLoad);
    }
    // Only viable cast for scalars: int -> real
    else if(castType == "real") {
        if(lTypeString == "int") {
            return it->makePair(ir->CreateSIToFP(lValueLoad, realTy, "upCastIntToReal"), rValueLoad);
        }
        else {
            return it->makePair(lValueLoad, ir->CreateSIToFP(rValueLoad, realTy, "upCastIntToreal"));
        }
    }
    else {
        auto *error = new ScalarErrorNode(lTypeString, rTypeString, line);
        eb->printError(error);

        return it->makePair(lValueLoad, rValueLoad);
    }
}

// This function is for casting with the use of the keyword: as
llvm::Value *CastTable::varCast(llvm::Type *type, llvm::Value *exprLoad, int line) {
    auto trueValue = static_cast<uint64_t>(static_cast<int64_t>(0));
    llvm::Value *zero = llvm::ConstantInt::get(i32Ty, trueValue);

    // GazpreaType of expr
    llvm::Type *exprType = exprLoad->getType();

    // Position in table
    int exprPos = getType(exprType);
    int typePos = getType(type);

    // Option for explicit casting
    std::string exprString = typeTable[exprPos][exprPos];
    std::string typeString = typeTable[exprPos][typePos];
    std::string realString = typeTable[typePos][typePos];

    // Casting to same type
    if(typeString == exprString) {
        return exprLoad;
    }

    // Casting expr to bool
    else if(typeString == "bool"){
        if(exprString == "char") {
            llvm::Value *temp = ir->CreateZExt(exprLoad, intTy, "charToInt");
            return ir->CreateICmpNE(temp, zero, "charToBool");
        }
        else if(exprString == "int") {
            return ir->CreateICmpNE(exprLoad, zero, "intToBool");
        }
    }

    // Casting expr to char
    else if(typeString == "char") {
        if(exprString == "bool") {
            llvm::Value *temp = ir->CreateZExt(exprLoad, intTy, "boolToInt");
            return ir->CreateTrunc(temp, charTy, "intToChar");
        }
        else if(exprString == "int") {
            auto *cb = new CondBuilder(globalCtx, ir, mod);

            llvm::Value *exprStore = ir->CreateAlloca(intTy);
            ir->CreateStore(exprLoad, exprStore);

            // If integer is negative, perform subtraction
            // Otherwise, skip
            llvm::Value *cond = ir->CreateICmpSLT(exprLoad, zero);
            cb->beginIf(cond, "intToChar");;

            llvm::Value *temp = ir->CreateSub(zero, exprLoad);
            ir->CreateStore(temp, exprStore);
            cb->endIf();
            cb->finalize("intToChar");

            exprLoad = ir->CreateLoad(exprStore);
            return ir->CreateTrunc(exprLoad, charTy, "intToChar");
        }
    }

    // Casting expr to int
    else if(typeString == "int") {
        if(exprString == "bool") {
            return ir->CreateZExt(exprLoad, intTy, "boolToInt");
        }
        else if(exprString == "char") {
            return ir->CreateSExt(exprLoad, intTy, "charToInt");
        }
        else if(exprString == "real") {
            return ir->CreateFPToSI(exprLoad, intTy, "realToInt");
        }
    }

    // Casting expr to real
    else if(typeString == "real") {
        if(exprString == "bool") {
            return ir->CreateUIToFP(exprLoad, realTy, "boolToReal");
        }
        else if(exprString == "char") {
            return ir->CreateUIToFP(exprLoad, realTy, "charToReal");
        }
        else if(exprString == "int") {
            return ir->CreateSIToFP(exprLoad, realTy, "intToReal");
        }
    }

    else {
        auto *error = new ScalarErrorNode(realString, exprString, line);
        eb->printError(error);
    }
    return nullptr;
}

llvm::Value *CastTable::typeAssCast(llvm::Type *type, llvm::Value *expr, int line, llvm::Value *size, int exprSize) {
    //auto *cb = new CondBuilder(globalCtx, ir, mod);

    if(expr->getName() == "IdnNode")
        expr = it->getIdn(type);

    // Deals with type assignment when declaration type is of vector type
    if(it->isDeclVectorType(type)) {
        return vecAssCast(type, expr, line, size, exprSize);
    }

    llvm::Type *rTypeP = expr->getType();

    // Position in typeTable
    int lType = getType(type);
    int rType = getType(rTypeP);

    // Cast type
    std::string castType = typeAssTable[lType][rType];
    std::string lTypeString = typeAssTable[lType][lType];
    std::string rTypeString = typeAssTable[rType][rType];

    if(castType == lTypeString && castType == rTypeString) {
        return expr;
    }
    // Only viable cast for scalars: int -> real
    else if(castType == "real") {
        return ir->CreateSIToFP(expr, realTy, "upCastIntToReal");
    }
    else {
        auto *error = new ScalarErrorNode(lTypeString, rTypeString, line);
        eb->printError(error);
    }
    return nullptr;
}

llvm::Value *CastTable::vecAssCast(llvm::Type *type, llvm::Value *expr, int line, llvm::Value *size, int exprSize) {
    llvm::Value *declType = it->getConstFromType(it->getDeclScalarTypeFromVec(type));

    llvm::Type *llType = it->getDeclScalarTypeFromVec(type);
    int lType = getType(llType);

    std::string lTypeString = typeAssTable[lType][lType];
    std::string rTypeString;

    // First case: Expr is a vector and size exist
    if (it->isVectorType(expr) && size) {
        // Checks if types are the same
        if (expr->getType() == type) {
           llvm::Value *vec = et->getNewVector(declType);
            vec = it->castVectorToType(vec, it->getDeclScalarTypeFromVec(type));
            et->initVector(vec, size);

            et->strictCopyVectorElements(vec, expr, it->getConsi32(line));

            return vec;

        } else if (expr->getType() == intVecTy->getPointerTo()) {
           // Creates new vector
            llvm::Value *vec = et->getNewVector(declType);
            vec = it->castVectorToType(vec, it->getDeclScalarTypeFromVec(type));
            et->initVector(vec, size);

            if (type == realVecTy) {
                expr = createVecFromVec(expr, realTy, size, line);
            } else {
                llvm::Type *rrType = it->getDeclScalarTypeFromVec(expr->getType()->getPointerElementType());
                int rType = getType(rrType);
                rTypeString = typeAssTable[rType][rType];

                auto *error = new VectorErrorNode(lTypeString, rTypeString, (int)llvm::dyn_cast<llvm::ConstantInt>(size)->getSExtValue(), exprSize, line);
                eb->printError(error);
            }

            et->strictCopyVectorElements(vec, expr, it->getConsi32(line));

            return vec;
        } else {
            llvm::Type *rrType = it->getDeclScalarTypeFromVec(expr->getType());
            int rType = getType(rrType);
            rTypeString = typeAssTable[rType][rType];

            auto *error = new VectorErrorNode(lTypeString, rTypeString, (int)llvm::dyn_cast<llvm::ConstantInt>(size)->getSExtValue(), exprSize, line);
            eb->printError(error);
        }
    }
    // Second case: Expr is a vector and size doesn't exist
    else if (it->isVectorType(expr) && !size) {
        if (expr->getType() == type->getPointerTo()) {
            llvm::Value *exprlSize = it->getValFromStruct(expr, it->getConsi32(VEC_LEN_INDEX));

            llvm::Value *vec = et->getNewVector(declType);
            vec = it->castVectorToType(vec, it->getDeclScalarTypeFromVec(type));
            et->initVector(vec, exprlSize);

            et->strictCopyVectorElements(vec, expr, it->getConsi32(line));

            return vec;

        } else if (expr->getType() == intVecTy->getPointerTo()) {
            llvm::Value *exprlSize = it->getValFromStruct(expr, it->getConsi32(VEC_LEN_INDEX));

            // Creates new vector
            llvm::Value *vec = et->getNewVector(declType);
            vec = it->castVectorToType(vec, it->getDeclScalarTypeFromVec(type));
            et->initVector(vec, exprlSize);

            if (type == realVecTy) {
                expr = createVecFromVec(expr, realTy, exprlSize, line);
            } else {
                llvm::Type *rrType = it->getDeclScalarTypeFromVec(expr->getType()->getPointerElementType());
                int rType = getType(rrType);
                rTypeString = typeAssTable[rType][rType];

                auto *error = new VectorErrorNode(lTypeString, rTypeString, exprSize, false, line);
                eb->printError(error);
            }

            et->strictCopyVectorElements(vec, expr, it->getConsi32(line));

            return vec;
        } else {
            llvm::Type *rrType = it->getDeclScalarTypeFromVec(expr->getType()->getPointerElementType());
            int rType = getType(rrType);
            rTypeString = typeAssTable[rType][rType];

            auto *error = new VectorErrorNode(lTypeString, rTypeString, exprSize, false, line);
            eb->printError(error);
        }
    }
    // Third case: Expr is a scalar and size exist
    else if (size && !it->isVectorType(expr)) {
        if(it->getDeclScalarTypeFromVec(type) == expr->getType()) {
            llvm::Value *vec;

            vec = createVecFromScalar(expr, it->getDeclScalarTypeFromVec(type), size, line);

            return vec;
        } else if (expr->getType() == intTy) {
            // Creates new vector
            llvm::Value *vec = et->getNewVector(declType);
            vec = it->castVectorToType(vec, it->getDeclScalarTypeFromVec(type));
            et->initVector(vec, size);

            if (type == realVecTy) {
                expr = createVecFromScalar(expr, realTy, size, line);
            } else {
                // Todo: make error node for trying to implicit cast scalar integer to vector types other than real and int
                llvm::Type *rrType = expr->getType();
                int rType = getType(rrType);
                rTypeString = typeAssTable[rType][rType];

                auto *error = new VectorErrorNode(lTypeString, rTypeString, (int)llvm::dyn_cast<llvm::ConstantInt>(size)->getSExtValue(), false, line);
                eb->printError(error);
            }

            et->strictCopyVectorElements(vec, expr, it->getConsi32(line));

            return vec;
        } else {
            // Todo: make error node for trying to implicit cast scalar to vector types other than real and int
        }
    }
    // Fourth case: Expr is a scalar and size doesn't exist
    else {

        // Todo: Make error node for rest of types
    }
}


llvm::Value *CastTable::createVecFromScalar(llvm::Value *exprP, llvm::Type *type, llvm::Value *size, int line) {
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    llvm::Value *elemValue = varCast(type, exprP, line);
    llvm::Value *newVecElemPtr;

    // Create new empty vector
    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));
    vec = it->castVectorToType(vec, type);

    // Counter
    llvm::Value *curVecSize = it->getConsi32(0);
    llvm::Value *curVecPtr = ir->CreateAlloca(intTy);

    ir->CreateStore(curVecSize, curVecPtr);

    // Creates empty vector of cast type and size of expr
    et->initVector(vec, size);

    wb->beginWhile();

    curVecSize = ir->CreateLoad(curVecPtr);
    llvm::Value *cmpStatement = ir->CreateICmpSLT(curVecSize, size, "CompStatement");

    // Body
    wb->insertControl(cmpStatement);

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

llvm::Value *CastTable::createVecFromVec(llvm::Value *exprP, llvm::Type *type, llvm::Value *maxSize, int line) {
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    llvm::Value *elemPtr;
    llvm::Value *elemValue;
    llvm::Value *newVecElemPtr;
    llvm::Value *elements = it->getPtrFromStruct(exprP, it->getConsi32(VEC_ELEM_INDEX));

    // Create new empty vector
    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));
    vec = it->castVectorToType(vec, type);

    // Counter
    llvm::Value *curVecSize = it->getConsi32(0);
    llvm::Value *curVecPtr = ir->CreateAlloca(intTy);

    ir->CreateStore(curVecSize, curVecPtr);

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
    elemValue = varCast(type, elemValue, line);

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

InternalTools::pair CastTable::vectorTypePromotion(llvm::Value *lValueLoad, llvm::Value *rValueLoad, int line) {
    int lType;
    int rType;
    llvm::Type *lTypeP;
    llvm::Type *rTypeP;
    std::string castType;
    std::string lTypeString;
    std::string rTypeString;
    llvm::Value *vec;
    llvm::Value *size;

    // Type promotion between scalar and vector where leftExpr is the vector
    if (lValueLoad->getType()->isPointerTy() && !rValueLoad->getType()->isPointerTy()) {
        lTypeP = it->getVectorElementType(lValueLoad);
        rTypeP = rValueLoad->getType();

        lType = getType(lTypeP);
        rType = getType(rTypeP);

        castType = typePTable[lType][rType];
        lTypeString = typePTable[lType][lType];
        rTypeString = typePTable[rType][rType];

        size = it->getValFromStruct(lValueLoad, it->getConsi32(VEC_LEN_INDEX));

        if (lTypeString == rTypeString) {
            rValueLoad = createVecFromScalar(rValueLoad, rTypeP, size, line);

            return it->makePair(lValueLoad, rValueLoad);
        } else if (castType == "real") {
            if (lTypeString == "int") {
                lValueLoad = createVecFromVec(lValueLoad, realTy, size, line);
                rValueLoad = createVecFromScalar(rValueLoad, realTy, size, line);

                return it->makePair(lValueLoad, rValueLoad);
            } else {
                rValueLoad = createVecFromScalar(rValueLoad, realTy, size, line);

                return it->makePair(lValueLoad, rValueLoad);
            }
        } else {
            // Todo: make new error node for vectors
            auto *error = new ScalarErrorNode(lTypeString, rTypeString, line);
            eb->printError(error);

            return it->makePair(lValueLoad, rValueLoad);
        }
    }

    // Type promotion between scalar and vector where rightExpr is vector
    else if (rValueLoad->getType()->isPointerTy() && !lValueLoad->getType()->isPointerTy()) {
        lTypeP = lValueLoad->getType();
        rTypeP = it->getVectorElementType(rValueLoad);

        lType = getType(lTypeP);
        rType = getType(rTypeP);

        castType = typePTable[lType][rType];
        lTypeString = typePTable[lType][lType];
        rTypeString = typePTable[rType][rType];

        size = it->getValFromStruct(rValueLoad, it->getConsi32(VEC_LEN_INDEX));

        if (lTypeString == rTypeString) {
            lValueLoad = createVecFromScalar(lValueLoad, lTypeP, size, line);

            return it->makePair(lValueLoad, rValueLoad);
        } else if (castType == "real") {
            if (lTypeString == "int") {
                lValueLoad = createVecFromScalar(lValueLoad, realTy, size, line);

                return it->makePair(lValueLoad, rValueLoad);
            } else {
                lValueLoad = createVecFromScalar(lValueLoad, realTy, size, line);
                rValueLoad = createVecFromVec(rValueLoad, realTy, size, line);

                return it->makePair(lValueLoad, rValueLoad);
            }
        } else {
            // Todo: make new error node for vectors
            auto *error = new ScalarErrorNode(lTypeString, rTypeString, line);
            eb->printError(error);

            return it->makePair(lValueLoad, rValueLoad);
        }

    // Type promotion between vector and vector
    } else {
        auto cb = new CondBuilder(globalCtx, ir, mod);

        lTypeP = it->getVectorElementType(lValueLoad);
        rTypeP = it->getVectorElementType(rValueLoad);

        lType = getType(lTypeP);
        rType = getType(rTypeP);

        castType = typePTable[lType][rType];
        lTypeString = typePTable[lType][lType];
        rTypeString = typePTable[rType][rType];

        llvm::Value *lSize = it->getValFromStruct(lValueLoad, it->getConsi32(VEC_LEN_INDEX));
        llvm::Value *rSize = it->getValFromStruct(rValueLoad, it->getConsi32(VEC_LEN_INDEX));

        size = ir->CreateAlloca(intTy);
        llvm::Value *cond = ir->CreateICmpSLT(lSize, rSize);
        cb->beginIf(cond);

        // Set size as rightExpr
        ir->CreateStore(rSize, size);

        cb->endIf();
        cb->beginElse();

        // Set size as leftExpr
        ir->CreateStore(lSize, size);

        cb->finalize();

        size = ir->CreateLoad(size);

        if (lTypeString == rTypeString) {
            lValueLoad = createVecFromVec(lValueLoad, rTypeP, size, line);
            rValueLoad = createVecFromVec(rValueLoad, rTypeP, size, line);

            return it->makePair(lValueLoad, rValueLoad);
        } else if (castType == "real") {
            if (lTypeString == "int") {
                lValueLoad = createVecFromVec(lValueLoad, realTy, size, line);
                rValueLoad = createVecFromVec(rValueLoad, realTy, size, line);

                return it->makePair(lValueLoad, rValueLoad);
            } else {
                // Todo: make new error node for vectors
                auto *error = new ScalarErrorNode(lTypeString, rTypeString, line);
                eb->printError(error);

                return it->makePair(lValueLoad, rValueLoad);
            }
        }
    }

}

