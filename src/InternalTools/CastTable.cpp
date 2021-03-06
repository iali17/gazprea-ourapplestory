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
InternalTools::pair CastTable::typePromotion(llvm::Value *lValueLoad, llvm::Value *rValueLoad, int line, int opt) {
    assert(!((lValueLoad == nullptr) && (rValueLoad == nullptr)));

    if(!lValueLoad) {
        if (it->isMatrixType(rValueLoad)) {
            llvm::Type *rType = it->getMatrixElementType(rValueLoad);
            llvm::Value *rRowSize = it->getValFromStruct(rValueLoad, MATRIX_NUMROW_INDEX);
            llvm::Value *rColSize = it->getValFromStruct(rValueLoad, MATRIX_NUMCOL_INDEX);

            lValueLoad = et->getNewMatrix(it->getConstFromType(rType));
            et->initMatrix(lValueLoad, rRowSize, rColSize);
            lValueLoad = it->castMatrixToType(lValueLoad, rType);

            et->setNullMatrix(lValueLoad);
        } else if (it->isVectorType(rValueLoad)) {
            llvm::Type *rType = it->getVectorElementType(rValueLoad);
            llvm::Value *rSize = it->getValFromStruct(rValueLoad, VEC_LEN_INDEX);

            lValueLoad = et->getNewVector(it->getConstFromType(rType));
            et->initVector(lValueLoad, rSize);
            lValueLoad = it->castVectorToType(lValueLoad, rType);

            et->setNullVector(lValueLoad);
        } else if (it->isIntervalType(rValueLoad)) {
            lValueLoad = et->getNewInterval(it->getConsi32(0), it->getConsi32(0));
        } else
            lValueLoad = it->getNull(rValueLoad->getType());
    }
    else if(!rValueLoad) {
        if(it->isMatrixType(lValueLoad)) {
            llvm::Type *lType = it->getMatrixElementType(lValueLoad);
            llvm::Value *lRowSize = it->getValFromStruct(lValueLoad, MATRIX_NUMROW_INDEX);
            llvm::Value *lColSize = it->getValFromStruct(lValueLoad, MATRIX_NUMCOL_INDEX);

            rValueLoad = et->getNewMatrix(it->getConstFromType(lType));
            et->initMatrix(rValueLoad, lRowSize, lColSize);
            rValueLoad = it->castMatrixToType(rValueLoad, lType);

            et->setNullMatrix(rValueLoad);
        } else if(it->isVectorType(lValueLoad)) {
            llvm::Type *lType = it->getVectorElementType(lValueLoad);
            llvm::Value *lSize = it->getValFromStruct(lValueLoad, VEC_LEN_INDEX);

            rValueLoad = et->getNewVector(it->getConstFromType(lType));
            et->initVector(rValueLoad, lSize);
            rValueLoad = it->castVectorToType(rValueLoad, lType);

            et->setNullVector(rValueLoad);
        } else if(it->isIntervalType(lValueLoad)) {
            rValueLoad = et->getNewInterval(it->getConsi32(0), it->getConsi32(0));
        } else
            rValueLoad = it->getNull(lValueLoad->getType());
    }
    else if(lValueLoad->getName() == "IdnNode") {
        if(it->isMatrixType(rValueLoad)) {
            llvm::Type *rType = it->getMatrixElementType(rValueLoad);
            llvm::Value *rRowSize = it->getValFromStruct(rValueLoad, MATRIX_NUMROW_INDEX);
            llvm::Value *rColSize = it->getValFromStruct(rValueLoad, MATRIX_NUMCOL_INDEX);

            lValueLoad = et->getNewMatrix(it->getConstFromType(rType));
            et->initMatrix(lValueLoad, rRowSize, rColSize);
            lValueLoad = it->castMatrixToType(lValueLoad, rType);

            et->setIdentityMatrix(lValueLoad);
        } else if(it->isVectorType(rValueLoad)) {
            llvm::Type *rType = it->getVectorElementType(rValueLoad);
            llvm::Value *rSize = it->getValFromStruct(rValueLoad, VEC_LEN_INDEX);

            lValueLoad = et->getNewVector(it->getConstFromType(rType));
            et->initVector(lValueLoad, rSize);
            lValueLoad = it->castVectorToType(lValueLoad, rType);

            et->setIdentityVector(lValueLoad);
        } else if(it->isIntervalType(rValueLoad)) {
            lValueLoad = et->getNewInterval(it->getConsi32(1), it->getConsi32(1));
        } else
            lValueLoad = it->getIdn(rValueLoad->getType());
    }
    else if(rValueLoad->getName() == "IdnNode") {
        if (it->isMatrixType(lValueLoad)) {
            llvm::Type *lType = it->getMatrixElementType(lValueLoad);
            llvm::Value *lRowSize = it->getValFromStruct(lValueLoad, MATRIX_NUMROW_INDEX);
            llvm::Value *lColSize = it->getValFromStruct(lValueLoad, MATRIX_NUMCOL_INDEX);

            rValueLoad = et->getNewMatrix(it->getConstFromType(lType));
            et->initMatrix(rValueLoad, lRowSize, lColSize);
            rValueLoad = it->castMatrixToType(rValueLoad, lType);

            et->setIdentityMatrix(rValueLoad);
        } else if (it->isVectorType(lValueLoad)) {
            llvm::Type *lType = it->getVectorElementType(lValueLoad);
            llvm::Value *lSize = it->getValFromStruct(lValueLoad, VEC_LEN_INDEX);

            rValueLoad = et->getNewVector(it->getConstFromType(lType));
            et->initVector(rValueLoad, lSize);
            rValueLoad = it->castVectorToType(rValueLoad, lType);

            et->setIdentityVector(rValueLoad);
        } else if(it->isIntervalType(lValueLoad)) {
            rValueLoad = et->getNewInterval(it->getConsi32(1), it->getConsi32(1));
        } else
            rValueLoad = it->getIdn(lValueLoad->getType());
    }

    if(it->isVectorType(lValueLoad) || it->isVectorType(rValueLoad)) {
        return vectorTypePromotion(lValueLoad, rValueLoad, line, opt);
    }

    if (it->isIntervalType(lValueLoad) || it->isIntervalType(rValueLoad)){
        return IntervalTypePromotion(lValueLoad, rValueLoad, line);
    }

    if(it->isMatrixType(lValueLoad) || it->isMatrixType(rValueLoad)) {
        return matrixTypePromotion(lValueLoad, rValueLoad, line, opt);
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

llvm::Value *CastTable::typeAssCast(llvm::Type *type, llvm::Value *expr, int line, llvm::Value *size, llvm::Value *extraSize, int exprSize) {
    if(expr->getName() == "IdnNode")
        expr = it->getIdn(type);

    // Deals with type assignment when declaration type is of vector type
    if(it->isDeclVectorType(type)) {
        return vecAssCast(type, expr, line, size, exprSize);
    }

    // Deals with type assignment when declaration type is of matrix type
    if(it->isDeclMatrixType(type)) {
        return matAssCast(type, expr, line, size, extraSize);
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
        if (expr->getType() == type->getPointerTo()) {
           llvm::Value *vec = et->getNewVector(declType);
            vec = it->castVectorToType(vec, it->getDeclScalarTypeFromVec(type));
            et->initVector(vec, size);

            et->strictCopyVectorElements(vec, expr, it->getConsi32(line), it->getConsi32(true));

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

            et->strictCopyVectorElements(vec, expr, it->getConsi32(line), it->getConsi32(true));

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
        if (expr->getType() == type->getPointerTo() || expr->getType() == type) {
            llvm::Value *exprlSize = it->getValFromStruct(expr, it->getConsi32(VEC_LEN_INDEX));

            llvm::Value *vec = et->getNewVector(declType);
            vec = it->castVectorToType(vec, it->getDeclScalarTypeFromVec(type));
            et->initVector(vec, exprlSize);

            et->strictCopyVectorElements(vec, expr, it->getConsi32(line), it->getConsi32(true));

            return vec;

        } else if (expr->getType() == intVecTy->getPointerTo()) {
            llvm::Value *exprlSize = it->getValFromStruct(expr, it->getConsi32(VEC_LEN_INDEX));

            // Creates new vector
            llvm::Value *vec = et->getNewVector(declType);
            vec = it->castVectorToType(vec, it->getDeclScalarTypeFromVec(type));
            et->initVector(vec, exprlSize);

            if (type == realVecTy->getPointerTo() || type == realVecTy) {
                expr = createVecFromVec(expr, realTy, exprlSize, line);
            } else {
                llvm::Type *rrType = it->getDeclScalarTypeFromVec(expr->getType()->getPointerElementType());
                int rType = getType(rrType);
                rTypeString = typeAssTable[rType][rType];

                auto *error = new VectorErrorNode(lTypeString, rTypeString, exprSize, false, line);
                eb->printError(error);
            }

            et->strictCopyVectorElements(vec, expr, it->getConsi32(line), it->getConsi32(true));

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
                llvm::Type *rrType = expr->getType();
                int rType = getType(rrType);
                rTypeString = typeAssTable[rType][rType];

                auto *error = new VectorErrorNode(lTypeString, rTypeString, (int)llvm::dyn_cast<llvm::ConstantInt>(size)->getSExtValue(), true, line);
                eb->printError(error);
            }

            et->strictCopyVectorElements(vec, expr, it->getConsi32(line), it->getConsi32(true));

            return vec;
        } else if(it->isIntervalType(expr)) {
            llvm::Value *vec = et->getNewVector(declType);
            vec = it->castVectorToType(vec, it->getDeclScalarTypeFromVec(type));
            et->initVector(vec, size);

            // Creates vector from interval
            llvm::Value *newVec = et->getVectorFromInterval(expr, it->getConsi32(1));
            newVec = ir->CreatePointerCast(newVec, intVecTy->getPointerTo());
            llvm::Value *intervalSize = it->getValFromStruct(newVec, it->getConsi32(VEC_LEN_INDEX));

            if(type == intVecTy) {
                et->strictCopyVectorElements(vec, newVec, it->getConsi32(line), it->getConsi32(true));

                return vec;
            } else if(type == realVecTy) {
                newVec  = createVecFromVec(newVec, realTy, intervalSize, line);
                et->strictCopyVectorElements(vec, newVec, it->getConsi32(line), it->getConsi32(true));

                return vec;
            } else {
                llvm::Type *rrType = it->getValFromStruct(newVec, it->getConsi32(VEC_TYPE_INDEX))->getType();
                int rType = getType(rrType);
                rTypeString = typeAssTable[rType][rType];

                auto *error = new VectorErrorNode(lTypeString, rTypeString, (int)llvm::dyn_cast<llvm::ConstantInt>(size)->getSExtValue(), -1,line);
                eb->printError(error);
            }
        } else {
            llvm::Type *rrType = expr->getType();
            int rType = getType(rrType);
            rTypeString = typeAssTable[rType][rType];

            auto *error = new VectorErrorNode(lTypeString, rTypeString, (int)llvm::dyn_cast<llvm::ConstantInt>(size)->getSExtValue(), true, line);
            eb->printError(error);
        }
    }
    // Fourth case: Expr is a scalar and size doesn't exist
    else {
        if(it->isIntervalType(expr)) {
            // Creates vector from interval
            llvm::Value *newVec = et->getVectorFromInterval(expr, it->getConsi32(1));
            newVec = ir->CreatePointerCast(newVec, intVecTy->getPointerTo());
            llvm::Value *intervalSize = it->getValFromStruct(newVec, it->getConsi32(VEC_LEN_INDEX));

            if(type == intVecTy) {
                return newVec;
            } else if(type == realVecTy) {
                newVec  = createVecFromVec(newVec, realTy, intervalSize, line);

                return newVec;
            } else {
                llvm::Type *rrType = it->getValFromStruct(newVec, it->getConsi32(VEC_TYPE_INDEX))->getType();
                int rType = getType(rrType);
                rTypeString = typeAssTable[rType][rType];

                auto *error = new VectorErrorNode(lTypeString, rTypeString, (int)llvm::dyn_cast<llvm::ConstantInt>(size)->getSExtValue(), false, line);
                eb->printError(error);
            }
        }

        llvm::Type *rrType = expr->getType();
        int rType = getType(rrType);
        rTypeString = typeAssTable[rType][rType];

        auto *error = new VectorErrorNode(lTypeString, rTypeString, line);
        eb->printError(error);
    }
    std::cout << "unknown error in cast table\n"; exit(1);
}

llvm::Value *CastTable::matAssCast(llvm::Type *type, llvm::Value *expr, int line, llvm::Value *leftSize, llvm::Value *rightSize) {
    llvm::Value *mat;
    llvm::Type *llType = it->getDeclScalarTypeFromMat(type);
    //llvm::Value *declType = it->getConstFromType(type);

    int lType = getType(llType);
    int rType;

    std::string lTypeString = typeAssTable[lType][lType];
    std::string rTypeString;
    std::string castType;

    // Deals with casting matrix to matrix
    if(it->isMatrixType(expr)) {

        llvm::Type *exprType = it->getMatrixElementType(expr);

        rType = getType(exprType);
        rTypeString = typeAssTable[rType][rType];
        castType = typeAssTable[lType][rType];

        if(llType == exprType) {
            mat = createMatViaSizeGiven(mat, expr, llType, leftSize, rightSize, line);

            return mat;
        } else if(castType == "real") {
            mat = createMatViaSizeGiven(mat, expr, realTy, leftSize, rightSize, line);

            return mat;

        } else {
            //todo: create error node
            std::cerr << "Casting matrix to different invalid matrix, make node later\n";
            exit(1);
        }

    }

    // Todo: change vec size once i get it
    // Specifically used for slicing promotion
    else if(it->isVectorType(expr)) {
        llvm::Type *exprType = it->getVectorElementType(expr);

        if(llType == exprType) {
            return expr;
        } else if(llType == realTy && exprType == intTy) {
            return createVecFromVec(expr, realTy, it->getValFromStruct(expr, VEC_LEN_INDEX), line);
        } else {
            std::cout << "unknown error in cast table\n";
        }

        return expr;
    }

    // Deals with casting scalars to matrix
    else {
        if(leftSize && rightSize)
            return createMatFromScalar(expr, llType, leftSize, rightSize, line);
        else {
            std::cerr << "No size given, add error node later\n";
            exit(1);
        }
    }
}

llvm::Value *CastTable::createMatViaSizeGiven(llvm::Value *mat, llvm::Value *expr, llvm::Type *type, llvm::Value *rowSize, llvm::Value *colSize, int line) {
    llvm::Value *exprRows = it->getValFromStruct(expr, MATRIX_NUMROW_INDEX);
    llvm::Value *exprCols = it->getValFromStruct(expr, MATRIX_NUMCOL_INDEX);

    if(rowSize && colSize) {
        return createMatFromMat(expr, type, rowSize, colSize, line);
    } else if(rowSize) {
        return createMatFromMat(expr, type, rowSize, exprCols, line);
    } else if(colSize) {
        return createMatFromMat(expr, type, exprRows, colSize, line);
    } else {
        return createMatFromMat(expr, type, exprRows, exprCols, line);
    }
}

llvm::Value *CastTable::createVecFromScalar(llvm::Value *exprP, llvm::Type *type, llvm::Value *size, int line) {
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    llvm::Value *elemValue = varCast(type, exprP, line);
    llvm::Value *newVecElemPtrStruct;
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

    // Get ptr to position in memory where element is stored
    newVecElemPtrStruct = it->getPtrFromStruct(vec, it->getConsi32(VEC_ELEM_INDEX));

    wb->beginWhile();

    curVecSize = ir->CreateLoad(curVecPtr);
    llvm::Value *cmpStatement = ir->CreateICmpSLT(curVecSize, size, "CompStatement");

    // Body
    wb->insertControl(cmpStatement);

    newVecElemPtr = ir->CreateGEP(newVecElemPtrStruct, curVecSize);

    // Store casted value into new vector at current position
    ir->CreateStore(elemValue, newVecElemPtr);

    // Increment counter
    curVecSize = ir->CreateAdd(curVecSize, it->getConsi32(1));
    ir->CreateStore(curVecSize, curVecPtr);

    wb->endWhile();

    return vec;
}

llvm::Value *CastTable::createMatFromScalar(llvm::Value *expr, llvm::Type *type, llvm::Value *leftSize, llvm::Value *rightSize, int line) {
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    llvm::Type *elemType = it->getDeclScalarTypeFromMat(type);
    llvm::Value *curVec;
    llvm::Value *mat;
    llvm::Value *exprVal;
    llvm::Value *curMatPtr;
    llvm::Value *ptr;

    // Create new empty matrix
    mat = et->getNewMatrix(it->getConstFromType(elemType));

    // Counter
    llvm::Value *curRowSize = it->getConsi32(0);
    llvm::Value *curRowPtr = ir->CreateAlloca(intTy);

    ir->CreateStore(curRowSize, curRowPtr);

    llvm::Value *exprP = ir->CreateAlloca(expr->getType());
    ir->CreateStore(expr, exprP);

    // Creates empty matrix to type and size given
    et->initMatrix(mat, leftSize, rightSize);
    mat = it->castMatrixToType(mat, elemType);

    // Ptr to current position in matrix
    ptr = it->getPtrFromStruct(mat, it->getConsi32(MATRIX_ELEM_INDEX));

    wb->beginWhile();

    curRowSize = ir->CreateLoad(curRowPtr);
    exprVal = ir->CreateLoad(exprP);

    llvm::Value *cmpStatement = ir->CreateICmpSLT(curRowSize, leftSize, "CompStatement");

    // Body
    wb->insertControl(cmpStatement);

    curVec = createVecFromScalar(exprVal, elemType, rightSize, line);
    curVec = ir->CreateLoad(curVec);

    // Store copy of vector into position of new matrix
    curMatPtr = ir->CreateGEP(ptr, curRowSize);
    ir->CreateStore(curVec, curMatPtr);

    // Increment counter
    curRowSize = ir->CreateAdd(curRowSize, it->getConsi32(1));
    ir->CreateStore(curRowSize, curRowPtr);

    ir->CreateStore(expr, exprP);

    wb->endWhile();

    return mat;
}

llvm::Value *CastTable::createVecFromVec(llvm::Value *exprP, llvm::Type *type, llvm::Value *maxSize, int line) {
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    llvm::Value *elemPtr;
    llvm::Value *elemValue;
    llvm::Value *newVecElemPtr;
    llvm::Type *exprPType = it->getVectorElementType(exprP);

    // Create new empty vector
    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));

    // Counter
    llvm::Value *curVecSize = it->getConsi32(0);
    llvm::Value *curVecPtr = ir->CreateAlloca(intTy);

    ir->CreateStore(curVecSize, curVecPtr);

    // Creates empty vector of cast type and size of expr
    et->initVector(vec, maxSize);
    vec = it->castVectorToType(vec, type);

    wb->beginWhile();

    curVecSize = ir->CreateLoad(curVecPtr);
    llvm::Value *cmpStatement = ir->CreateICmpSLT(curVecSize, maxSize, "CompStatement");

    // Body
    wb->insertControl(cmpStatement);

    // This loads a pointer to the position in the vector
    elemPtr = et->getVectorElementPointerSafe(exprP, curVecSize);
    elemPtr = ir->CreatePointerCast(elemPtr, exprPType->getPointerTo());
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

llvm::Value * CastTable::createMatFromMat(llvm::Value *exprP, llvm::Type *type, llvm::Value *leftSize, llvm::Value *rightSize, int line) {
    auto *cb = new CondBuilder(globalCtx, ir, mod);
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    llvm::Value *curVecElemPtr;
    llvm::Value *curVec;
    llvm::Value *rowsRemaining;
    llvm::Value *ptr;
    llvm::Value *curMatPtr;

    llvm::Value *oldMatValues = it->getPtrFromStruct(exprP, MATRIX_ELEM_INDEX);
    llvm::Value *numRowInMat = it->getValFromStruct(exprP, MATRIX_NUMROW_INDEX);
    llvm::Type *elemType = it->getDeclScalarTypeFromMat(type);

    // Create new empty matrix
    llvm::Value *mat = et->getNewMatrix(it->getConstFromType(type));

    // Counter
    llvm::Value *curRowSize = it->getConsi32(0);
    llvm::Value *curRowPtr = ir->CreateAlloca(intTy);

    // Max row size
    llvm::Value *rowMaxSize;
    llvm::Value *rowMaxSizePtr = ir->CreateAlloca(intTy);

    ir->CreateStore(curRowSize, curRowPtr);

    // Creates empty matrix of cast type and given size
    et->initMatrix(mat, leftSize, rightSize);
    mat = it->castMatrixToType(mat, elemType);

    ptr = it->getPtrFromStruct(mat, MATRIX_ELEM_INDEX);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Check if null padding in rows are needed
    llvm::Value *cond = ir->CreateICmpSLT(numRowInMat, leftSize);

    // If the current matrix has more or equal to the number of rows set maxRowSize to row count, else, set it to matrixRowCount
    cb->beginIf(cond, "ifCond");

    ir->CreateStore(numRowInMat, rowMaxSizePtr);

    cb->endIf();
    cb->beginElse("elseCond");

    ir->CreateStore(leftSize, rowMaxSizePtr);

    cb->finalize();
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Load max size
    rowMaxSize = ir->CreateLoad(rowMaxSizePtr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Begin loop to create vectors
    wb->beginWhile();

    curRowSize = ir->CreateLoad(curRowPtr);
    llvm::Value *cmpStatement = ir->CreateICmpSLT(curRowSize, rowMaxSize, "WhileCond");

    // Body
    wb->insertControl(cmpStatement);

    // Get Vector at row curRowSize
    curVecElemPtr = ir->CreateGEP(oldMatValues, curRowSize);

    // Calls createVecFromVec function which deals with nullpadding and truncating in the cols
    curVec = createVecFromVec(curVecElemPtr, elemType, rightSize, line);
    curVec = ir->CreateLoad(curVec);

    // Store copy of vector into position of new matrix
    curMatPtr = ir->CreateGEP(ptr, curRowSize);
    ir->CreateStore(curVec, curMatPtr);

    // Increment counter
    curRowSize = ir->CreateAdd(curRowSize, it->getConsi32(1));
    ir->CreateStore(curRowSize, curRowPtr);

    wb->endWhile();
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    llvm::Value *cond2 = ir->CreateICmpSLT(numRowInMat, leftSize);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // If matrix needed nullpadding, continue into if statment else, skip
    auto *cb2 = new CondBuilder(globalCtx, ir, mod);

    cb2->beginIf(cond2, "nullPadMatrix");

    rowsRemaining = ir->CreateSub(leftSize, numRowInMat);
    rowsRemaining->setName("rowsRemain");

    // Reset counter
    curRowSize = it->getConsi32(0);
    ir->CreateStore(curRowSize, curRowPtr);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    auto *wb2 = new WhileBuilder(globalCtx, ir, mod);

    // Begin loop to create vectors
    wb2->beginWhile();

    curRowSize = ir->CreateLoad(curRowPtr);
    cmpStatement = ir->CreateICmpSLT(curRowSize, rowsRemaining, "NullWhileCond");

    // Body
    wb2->insertControl(cmpStatement);

    // Create null padded rows
    curVec = et->getNewVector(it->getConstFromType(type));
    et->initVector(curVec, rightSize);
    curVec = it->castVectorToType(curVec, elemType);
    curVec = ir->CreateLoad(curVec);

    // Position in row to be padded
    llvm::Value *paddedIndex = ir->CreateAdd(numRowInMat, curRowSize);

    // Store copy of vector into position of new matrix
    curMatPtr = ir->CreateGEP(ptr, paddedIndex);
    ir->CreateStore(curVec, curMatPtr);

    // Increment counter
    curRowSize = ir->CreateAdd(curRowSize, it->getConsi32(1));
    ir->CreateStore(curRowSize, curRowPtr);

    wb2->endWhile();

    cb2->endIf();
    cb2->finalize();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    return mat;
}

InternalTools::pair CastTable::IntervalTypePromotion(llvm::Value *left, llvm::Value *right, int line) {

    if ((it->isIntervalType(left) && it->isMatrixType(right)) ||
        (it->isMatrixType(left) && it->isIntervalType(right)) ){
        std::cerr << "Cannot implicitly convert interval to matrix on line: " << line << ". Aborting...\n";
        exit(1);
    } else if (left->getType() == realTy || right->getType() == realTy){
        std::cerr << "Cannot implicitly convert real to interval on line: " << line << ". Aborting...\n";
        exit(1);
    }

    // if left or right is vecType, then use a by to change the interval to a vector and return both vectors
    // if left or right is intType, then cast the integer into interval
    llvm::Value * newLeft = left;
    llvm::Value * newRight = right;
    if (left->getType() == intTy){
        newLeft = et->getNewInterval(left, left);
        assert(it->isIntervalType(newLeft));
    } else if (right->getType() == intTy) {
        newRight = et->getNewInterval(right, right);
        assert(it->isIntervalType(newRight));
    } else if (it->isVectorType(left)){
        // todo i dont think casting interval to vector works just yetst
        newRight = et->getVectorBy(right, it->getConsi32(1));
        assert(it->isVectorType(newRight));
        return vectorTypePromotion(newLeft, newRight, line);
    } else if (it->isVectorType(right)){
        newLeft = et->getVectorBy(left, it->getConsi32(1));
        assert(it->isVectorType(newLeft));
        return vectorTypePromotion(newLeft, newRight, line);
    }

    return it->makePair(newLeft, newRight);
}


InternalTools::pair CastTable::vectorTypePromotion(llvm::Value *lValueLoad, llvm::Value *rValueLoad, int line, int opt) {
    int lType;
    int rType;
    llvm::Type *lTypeP;
    llvm::Type *rTypeP;
    std::string castType;
    std::string lTypeString;
    std::string rTypeString;
    llvm::Value *size;


    // Type promotion between vector and interval where leftExpr is vector and rightExpr is the interval
    if(it->isVectorType(lValueLoad) && it->isIntervalType(rValueLoad)) {
        // Create vector from interval
        rValueLoad = et->getVectorFromInterval(rValueLoad, it->getConsi32(1));
        rValueLoad = ir->CreatePointerCast(rValueLoad, intVecTy->getPointerTo());

        return vectorToVectorPromotion(lValueLoad, rValueLoad, line, opt);
    }

    // Type promotion between vector and interval where leftExpr is interval and rightExpr is the vector
    else if(it->isIntervalType(lValueLoad) && it->isVectorType(rValueLoad)) {
        // Create vector from interval
        lValueLoad = et->getVectorFromInterval(lValueLoad, it->getConsi32(1));
        lValueLoad = ir->CreatePointerCast(lValueLoad, intVecTy->getPointerTo());

        return vectorToVectorPromotion(lValueLoad, rValueLoad, line, opt);
    }

    // Type promotion between scalar and vector where leftExpr is the vector
    else if (it->isVectorType(lValueLoad) && !it->isVectorType(rValueLoad)) {
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
            // Todo: make new error node for vectors, somehow get size into error
            auto *error = new VectorErrorNode(lTypeString, rTypeString, line);
            eb->printError(error);

            return it->makePair(lValueLoad, rValueLoad);
        }
    }

    // Type promotion between scalar and vector where rightExpr is vector
    else if (it->isVectorType(rValueLoad) && !it->isVectorType(lValueLoad)) {
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
            // Todo: make new error node for vectors, somehow get size into error
            auto *error = new VectorErrorNode(lTypeString, rTypeString, line);
            eb->printError(error);

            return it->makePair(lValueLoad, rValueLoad);
        }


    }

    // Type promotion between vector and vector
    else {
       return vectorToVectorPromotion(lValueLoad, rValueLoad, line, opt);
    }

}

InternalTools::pair CastTable::matrixTypePromotion(llvm::Value *leftExpr, llvm::Value *rightExpr, int line, int opt) {
    int lType;
    int rType;
    llvm::Type *lTypeP;
    llvm::Type *rTypeP;
    std::string castType;
    std::string lTypeString;
    std::string rTypeString;

    // Type promotion between matrix and scalar, where the leftExpr is a matrix and the rightExpr is a scalar
    if (it->isMatrixType(leftExpr) && !it->isMatrixType(rightExpr)) {
        llvm::Value *rowSize = it->getValFromStruct(leftExpr, MATRIX_NUMROW_INDEX);
        llvm::Value *colSize = it->getValFromStruct(leftExpr, MATRIX_NUMCOL_INDEX);

        lTypeP = it->getMatrixElementType(leftExpr);
        rTypeP = rightExpr->getType();

        lType = getType(lTypeP);
        rType = getType(rTypeP);

        castType = typePTable[lType][rType];
        lTypeString = typePTable[lType][lType];
        rTypeString = typePTable[rType][rType];

        if (lTypeString == rTypeString) {
            leftExpr = createMatFromMat(leftExpr, lTypeP, rowSize, colSize, line);
            rightExpr = createMatFromScalar(rightExpr, rTypeP, rowSize, colSize, line);

            return it->makePair(leftExpr, rightExpr);
        } else if (castType == "real") {
            leftExpr = createMatFromMat(leftExpr, realTy, rowSize, colSize, line);
            rightExpr = createMatFromScalar(rightExpr, realTy, rowSize, colSize, line);

            return it->makePair(leftExpr, rightExpr);
        } else {
            // Todo: make new error node for matrix, somehow get size into error

            return it->makePair(leftExpr, rightExpr);
        }
    }

        // Type promotion between matrix and scalar, where the rightExpr is a matrix and the leftExpr is a scalar
    else if (it->isMatrixType(rightExpr) && !it->isMatrixType(leftExpr)) {
        llvm::Value *rowSize = it->getValFromStruct(rightExpr, MATRIX_NUMROW_INDEX);
        llvm::Value *colSize = it->getValFromStruct(rightExpr, MATRIX_NUMCOL_INDEX);

        lTypeP = leftExpr->getType();
        rTypeP = it->getMatrixElementType(rightExpr);

        lType = getType(lTypeP);
        rType = getType(rTypeP);

        castType = typePTable[lType][rType];
        lTypeString = typePTable[lType][lType];
        rTypeString = typePTable[rType][rType];

        if (lTypeString == rTypeString) {
            leftExpr = createMatFromScalar(leftExpr, lTypeP, rowSize, colSize, line);
            rightExpr = createMatFromMat(rightExpr, rTypeP, rowSize, colSize, line);

            return it->makePair(leftExpr, rightExpr);
        } else if (castType == "real") {
            leftExpr = createMatFromScalar(leftExpr, realTy, rowSize, colSize, line);
            rightExpr = createMatFromMat(rightExpr, realTy, rowSize, colSize, line);

            return it->makePair(leftExpr, rightExpr);
        } else {
            // Todo: make new error node for matrix, somehow get size into error

            return it->makePair(leftExpr, rightExpr);
        }
    }

    // Type promotion between two matrices
    else {
        return matrixToMatrixPromotion(leftExpr, rightExpr, line, opt);
    }
}

InternalTools::pair CastTable::vectorToVectorPromotion(llvm::Value *leftExpr, llvm::Value *rightExpr, int line, int opt) {
    int lType;
    int rType;
    llvm::Type *lTypeP;
    llvm::Type *rTypeP;
    std::string castType;
    std::string lTypeString;
    std::string rTypeString;
    llvm::Value *size;

    if(opt == -1) {
        // makes a copy to check size
        llvm::Value *leftExprCopy = et->getVectorCopy(leftExpr);
        llvm::Value *rightExprCopy = et->getVectorCopy(rightExpr);

        et->strictCopyVectorElements(leftExprCopy, rightExprCopy, it->getConsi32(line), it->getConsi32(false));
    }

    auto cb = new CondBuilder(globalCtx, ir, mod);

    lTypeP = it->getVectorElementType(leftExpr);
    rTypeP = it->getVectorElementType(rightExpr);

    lType = getType(lTypeP);
    rType = getType(rTypeP);

    castType = typePTable[lType][rType];
    lTypeString = typePTable[lType][lType];
    rTypeString = typePTable[rType][rType];

    llvm::Value *lSize = it->getValFromStruct(leftExpr, it->getConsi32(VEC_LEN_INDEX));
    llvm::Value *rSize = it->getValFromStruct(rightExpr, it->getConsi32(VEC_LEN_INDEX));

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
        leftExpr = createVecFromVec(leftExpr, rTypeP, size, line);
        rightExpr = createVecFromVec(rightExpr, rTypeP, size, line);

        return it->makePair(leftExpr, rightExpr);
    } else if (castType == "real") {
        if (lTypeString == "int" || rTypeString == "int") {
            leftExpr = createVecFromVec(leftExpr, realTy, size, line);
            rightExpr = createVecFromVec(rightExpr, realTy, size, line);

            return it->makePair(leftExpr, rightExpr);
        } else {
            // Todo: make new error node for vectors, somehow get size into error
            auto *error = new VectorErrorNode(lTypeString, rTypeString, line);
            eb->printError(error);

            return it->makePair(leftExpr, rightExpr);
        }
    } else {
        auto *error = new VectorErrorNode(lTypeString, rTypeString, line);
        eb->printError(error);

        return it->makePair(leftExpr, rightExpr);
    }
}

InternalTools::pair CastTable::matrixToMatrixPromotion(llvm::Value *leftExpr, llvm::Value *rightExpr, int line, int opt) {
    // Type variables initialization
    int lType;
    int rType;
    llvm::Type *lTypeP;
    llvm::Type *rTypeP;
    std::string castType;
    std::string lTypeString;
    std::string rTypeString;

    if(opt == -1) {
        // Makes copys to check if size are different
        llvm::Value *leftExprCopy = et->getMatrixCopy(leftExpr);
        llvm::Value *rightExprCopy = et->getMatrixCopy(rightExpr);

        et->strictCopyMatrixElements(leftExprCopy, rightExprCopy, it->getConsi32(line), it->getConsi32(false));
    }

    // llvm value initialization
    llvm::Value *maxRowSize = ir->CreateAlloca(intTy);
    llvm::Value *maxColSize = ir->CreateAlloca(intTy);
    llvm::Value *leftRowSize;
    llvm::Value *rightRowSize;
    llvm::Value *leftColSize;
    llvm::Value *rightColSize;

    // Get row and col size of each matrix
    leftRowSize = it->getValFromStruct(leftExpr, MATRIX_NUMROW_INDEX);
    rightRowSize = it->getValFromStruct(rightExpr, MATRIX_NUMROW_INDEX);
    leftColSize = it->getValFromStruct(leftExpr, MATRIX_NUMCOL_INDEX);
    rightColSize = it->getValFromStruct(rightExpr, MATRIX_NUMCOL_INDEX);

    // Get type of each expr and check cast type
    lTypeP = it->getMatrixElementType(leftExpr);
    rTypeP = it->getMatrixElementType(rightExpr);

    lType = getType(lTypeP);
    rType = getType(rTypeP);

    castType = typePTable[lType][rType];
    lTypeString = typePTable[lType][lType];
    rTypeString = typePTable[rType][rType];

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Get max row between the two matrices
    auto *cb = new CondBuilder(globalCtx, ir, mod);
    llvm::Value *cond = ir->CreateICmpSLT(leftRowSize, rightRowSize);

    cb->beginIf(cond);

    ir->CreateStore(rightRowSize, maxRowSize);

    cb->endIf();
    cb->beginElse();

    ir->CreateStore(leftRowSize, maxRowSize);

    cb->finalize();

    maxRowSize = ir->CreateLoad(maxRowSize);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Get max col between the two matrices
    auto *cb2 = new CondBuilder(globalCtx, ir, mod);
    llvm::Value *cond2 = ir->CreateICmpSLT(leftColSize, rightColSize);

    cb2->beginIf(cond2);

    ir->CreateStore(rightColSize, maxColSize);

    cb2->endIf();
    cb2->beginElse();

    ir->CreateStore(leftColSize, maxColSize);

    cb2->finalize();

    maxColSize = ir->CreateLoad(maxColSize);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    if(lTypeString == rTypeString) {
        leftExpr = createMatFromMat(leftExpr, lTypeP, maxRowSize, maxColSize, line);
        rightExpr = createMatFromMat(rightExpr, rTypeP, maxRowSize, maxColSize, line);

        return it->makePair(leftExpr, rightExpr);
    } else if(castType == "real") {
        leftExpr = createMatFromMat(leftExpr, realTy, maxRowSize, maxColSize, line);
        rightExpr = createMatFromMat(rightExpr, realTy, maxRowSize, maxColSize, line);

        return it->makePair(leftExpr, rightExpr);
    } else {
        //todo: make error node for different type promotion

        return it->makePair(leftExpr, rightExpr);
    }
}



