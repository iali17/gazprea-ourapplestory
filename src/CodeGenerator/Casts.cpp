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
    llvm::StructType *types = parseStructType(dynamic_cast<TupleTypeNode *>(dynamic_cast<TupleCastNode *>(node)->getTuple()));
    llvm::Value *exprP = visit(node->getExpr());
    llvm::Value *ptr = ir->CreateAlloca(types);


    // Checks if casting another type to a tuple
    if(!it->isStructType(exprP)) {
        std::string left = "tuple(*)";
        std::string right = it->getType(exprP->getType(), exprP);
        auto *error = new ScalarErrorNode(left, right, node->getLine()); // print error message and abort
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
    return initTuple(ptr, values);
}

/**
 * Deals with vector casting
 *
 * @param node
 * @return
 */
llvm::Value* CodeGenerator::vectorCasting(CastExprNode *node) {
    auto values = new std::vector<llvm::Value *>();
    llvm::Type *type;
    llvm::Value *expr;
    llvm::Value *exprP = visit(node->getExpr());

    ASTNode *vnode = dynamic_cast<VectorCastNode *>(node)->getVector();
    std::string stype = dynamic_cast<VectorTypeNode *>(vnode)->getStringType();
    type = it->getVectorType(stype);

    // Extension size if exists
    llvm::Value *size  = visit(dynamic_cast<VectorTypeNode *>(vnode)->getSize());

    // Checks if expr is null
    if(!exprP) {
        exprP = et->getNewVector(it->getConstFromType(type));
        exprP = it->castVectorToType(exprP, type);

        assert(size);

        et->initVector(exprP, size);
    }

    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));
    vec = it->castVectorToType(vec, type);

    // This is for cases such as: as<integer vector[3]>(1) and as<integer vector>(1)
    if(!it->isStructType(exprP)) {
        if(size) {
            et->initVector(vec, size);
            vec = it->castVectorToType(vec, type);

            for (unsigned long i = 0; i < llvm::dyn_cast<llvm::ConstantInt>(size)->getSExtValue(); i++) {
                expr = ct->varCast(type, exprP, node->getLine());
                values->push_back(expr);
            }

            it->setVectorValues(vec, values);
            return vec;
        } else {
            std::string castType = it->getType(type, nullptr);
            std::string exprType = it->getType(exprP->getType(), nullptr);

            auto *error = new VectorErrorNode(castType, exprType, node->getLine());
            eb->printError(error);
        }
    }

    // Todo: Add proper error handling for this case: as<character vector>([1.0,2.0])
    // This is for cases such as: as<integer vector>([1,2]) and as<integer vector[4]>([1,2])
    else if(it->isVectorType(exprP)){

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
    
    // This is for cases such as: as<integer vector>(1..3) and as<integer vector[2]>(1..3);
    else if(it->isIntervalType(exprP)) {
        // Create a integer vector by default
        vec = et->getVectorFromInterval(exprP, it->getConsi32(1));
        ir->CreatePointerCast(vec, intVecTy->getPointerTo());

        if(type == charTy || type == boolTy) {
            VectorErrorNode *error;
            std::string castType = it->getType(type, nullptr);
            std::string exprType = "integer interval";

            if(size) {
                int vecSize = (int)llvm::dyn_cast<llvm::ConstantInt>(size)->getSExtValue();
                error = new VectorErrorNode(castType, exprType, vecSize, true, node->getLine());
                eb->printError(error);
            }

            error = new VectorErrorNode(castType, exprType, node->getLine());
            eb->printError(error);
        }

        if(size) {
            return ct->createVecFromVec(vec, type, size, node->getLine());
        }

        return vec;
    } else {
        //todo: error
        std::cerr << "Add error\n";
        exit(1);
    }
}

/**
 * Deals with matrix casting
 *
 * @param node
 * @return
 */
llvm::Value* CodeGenerator::matrixCasting(CastExprNode *node) {
    llvm::Value *exprP = visit(node->getExpr());
    ASTNode *mnode = dynamic_cast<MatrixCastNode *>(node)->getMatrix();

    // Type of matrix
    std::string sType = ((MatrixTypeNode *) mnode)->getStringType();
    auto typeNames = split(sType, 'm');
    sType = typeNames[0] + "matrix";
    llvm::Type *type = it->getDeclScalarTypeFromMat(it->getDeclMatrixType(sType));

    // Extension size if exists
    llvm::Value *rowSize  = visit(((MatrixTypeNode *) mnode)->getLeft());
    llvm::Value *colSize  = visit(((MatrixTypeNode *) mnode)->getRight());

    // Checks if expr is null
    if(!exprP) {
        exprP = et->getNewMatrix(it->getConstFromType(type));
        exprP = it->castMatrixToType(exprP, type);

        assert(rowSize || colSize);

        et->initMatrix(exprP, rowSize, colSize);
    }

    // Handles matrix to matrix casting
    if(it->isMatrixType(exprP)) {
        llvm::Value *mat = matrixUndefSizeCast(exprP, type, rowSize, colSize, node->getLine());

        return mat;
    } else if(!it->isStructType(exprP)) {
        if(rowSize && colSize) {
            llvm::Value *mat = ct->createMatFromScalar(exprP, type, rowSize, colSize, node->getLine());

            return mat;
        } else {
            // Todo: create error node
            std::cerr << "Create error node for no size given in scalar to mat explicit cast\n";
            exit(1);
        }
    } else {
        // Todo: add error node for invalid cast
        std::cerr << "Create error node for invalid explicit cast to matrix type\n";
        exit(1);
    }
}

/**
 * Deals with Scalar casting
 *
 * @param node
 * @return
 */
llvm::Value* CodeGenerator::scalarCasting(CastExprNode *node) {
    llvm::Type *type;
    llvm::Value *expr;
    llvm::Value *exprP = visit(node->getExpr());
    GazpreaType *gazpreaType = symbolTable->resolveType(dynamic_cast<ScalarCastNode *>(node)->getTypeString());
    type = gazpreaType->getTypeDef();

    // Checks if null
    if(!exprP) {
        exprP = it->getNull(type);
    }

    // Casting error when casting tuple to scalar
    if(it->isStructType(exprP)) {
        std::string left = dynamic_cast<ScalarCastNode *>(node)->getTypeString();
        std::string right = "tuple(*)";
        auto *error = new ScalarErrorNode(left, right, node->getLine()); // print error message and abort
        eb->printError(error);
    }

    return ct->varCast(type, exprP, node->getLine());
}

/**
 * Deals with vector casting when declaration doesn't give it a size
 *
 * @param vec
 * @param exprP
 * @param type
 * @param line
 * @return llvm::Value *
 */
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

/**
 * Deals with matrix casting for given size declarations
 *
 * @param exprP
 * @param type
 * @param rowSize
 * @param colSize
 * @param line
 * @return
 */
llvm::Value *CodeGenerator::matrixUndefSizeCast(llvm::Value *exprP, llvm::Type *type, llvm::Value *rowSize, llvm::Value *colSize, int line) {
    llvm::Value *exprRows = it->getValFromStruct(exprP, MATRIX_NUMROW_INDEX);
    llvm::Value *exprCols = it->getValFromStruct(exprP, MATRIX_NUMCOL_INDEX);

    if(rowSize && colSize) {
        return ct->createMatFromMat(exprP, type, rowSize, colSize, line);
    } else if(rowSize) {
        return ct->createMatFromMat(exprP, type, rowSize, exprCols, line);
    } else if(colSize) {
        return ct->createMatFromMat(exprP, type, exprRows, colSize, line);
    } else {
        return ct->createMatFromMat(exprP, type, exprRows, exprCols, line);
    }
}
