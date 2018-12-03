
//
// Created by kyle on 20/11/18.
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
 *
 * @param node
 * @return - pointer to the new matrix
 */
llvm::Value *CodeGenerator::visit(MatrixNode *node) {
    //local vars

    //vector nodes and llvm pointers
    auto *vectorNodes   = node->getElements();
    auto *vectors       = new std::vector<llvm::Value *>;
    ASTNode *vectorNode = nullptr;
    llvm::Value *vPtr   = nullptr;

    //locals for matrix setup
    llvm::Value *numCols    = nullptr;
    llvm::Value *numRows    = nullptr;
    llvm::Value *numColsPtr = nullptr;
    llvm::Value *curCols    = nullptr;
    llvm::Value *consTy     = nullptr;
    llvm::Type  *vecTy      = nullptr;

    //return value
    llvm::Value *matrix = nullptr;

    //this code will not work for a completely empty matrix so
    assert(not(vectorNodes->empty()));

    //we will keep track of the longest vector in here
    numColsPtr = ir->CreateAlloca(intTy);
    ir->CreateStore(it->getConsi32(0), numColsPtr);

    //get vectors
    for(uint i = 0; i < vectorNodes->size(); i++){
        vectorNode = vectorNodes->at(i);
        vPtr       = visit(vectorNode);
        vectors->push_back(vPtr);

        //set the max num cols

        //get the column lengths to compare
        curCols = it->getValFromStruct(vPtr, it->getConsi32(1));
        numCols = ir->CreateLoad(numColsPtr);

        //store col length if max
        auto *cb = new CondBuilder(globalCtx, ir, mod);
        cb->beginIf(ir->CreateICmpSGT(curCols, numCols));
            ir->CreateStore(curCols, numColsPtr);
        cb->endIf();
        cb->finalize();
    }

    //now we know the max col size and the row size
    numCols = ir->CreateLoad(numColsPtr);
    numRows = it->getConsi32(vectorNodes->size());

    //init matrix

    //find the type and base on the first element
    vecTy   = it->getVectorElementType(vectors->at(0));
    consTy  = it->getConstFromType(vecTy);

    //create new matrix and initialize to the size
    matrix  = et->getNewMatrix(consTy);
    et->initMatrix(matrix, numRows, numCols);

    //cast matrix to proper type
    matrix = it->castMatrixToType(matrix, vecTy);

    //
    llvm::Value *ptr = it->getPtrFromStruct(matrix, it->getConsi32(MATRIX_ELEM_INDEX));
    llvm::Value *curRowPtr;

    //assign all of the matrix values
    for(uint i = 0; i < vectorNodes->size(); i++){
        curRowPtr = ir->CreateGEP(ptr, it->getConsi32(i));
        et->copyVectorElements(curRowPtr, vectors->at(i));
    }

    return matrix;
}

llvm::Value *CodeGenerator::visit(MatrixDeclNode *node) {
    MatrixNode *matrixNode = nullptr;
    llvm::Value *mat = nullptr;

    auto *matrixTypeSize = (MatrixTypeNode *) node->getMatrixType();
    llvm::Type *matrixType = it->getDeclMatrixType(node->getInitType());
    llvm::Type *matrixElemType = it->getDeclScalarTypeFromMat(matrixType);
    matrixNode = dynamic_cast<MatrixNode *>(node->getExpr());

    // Declaration row and col
    llvm::Value *declRowSize = visit(matrixTypeSize->getLeft());
    llvm::Value *declColSize = visit(matrixTypeSize->getRight());

    // Get new matrix and cast to proper type
    mat = et->getNewMatrix(it->getConstFromType(matrixElemType));
    mat = it->castMatrixToType(mat, matrixElemType);

    // Handles expr is identity or null
    if (dynamic_cast<IdnNode *>(node->getExpr())) {
        if(declRowSize && declColSize) {
            et->initMatrix(mat, declRowSize, declColSize);
            et->setIdentityMatrix(mat);
            symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), mat);
            return nullptr;
        } else {
            std::cerr << "Make error node for size not given \n";
            exit(1);
        }
    } else if (dynamic_cast<NullNode *>(node->getExpr())) {
        if(declRowSize && declColSize) {
            et->initMatrix(mat, declRowSize, declColSize);
            et->setNullMatrix(mat);
            symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), mat);
            return nullptr;
        } else {
            std::cerr << "Make error node for size not given \n";
            exit(1);
        }
    }

    // Handles cases when expr is a matrix
    if (matrixNode) {
        llvm::Value *matExpr = visit(node->getExpr());
        llvm::Value *vecExprSize = it->getValFromStruct(matExpr, MATRIX_ELEM_INDEX);

        // Max row and col of exprs
        llvm::Value *rowSize = it->getValFromStruct(visit(node->getExpr()), MATRIX_NUMROW_INDEX);
        llvm::Value *colSize = it->getValFromStruct(visit(node->getExpr()), MATRIX_NUMCOL_INDEX);

        // Initialize matrix to given size
        if(declRowSize && declColSize) {
            et->initMatrix(mat, declRowSize, declColSize);
            mat = ct->typeAssCast(matrixType, matExpr, node->getLine(), declRowSize, declColSize);
        } else if(declRowSize) {
            et->initMatrix(mat, declRowSize, colSize);
            mat = ct->typeAssCast(matrixType, matExpr, node->getLine(), declRowSize, colSize);
        } else if(declColSize) {
            et->initMatrix(mat, rowSize, declColSize);
            mat = ct->typeAssCast(matrixType, matExpr, node->getLine(), rowSize, declColSize);
        } else {
            et->initMatrix(mat, rowSize, colSize);
            mat = ct->typeAssCast(matrixType, matExpr, node->getLine(), rowSize, colSize);
        }
    }

    // Handles case when expr is not a matrix
    else if (node->getExpr()) {
        llvm::Value *regExpr = visit(node->getExpr());

        mat = ct->typeAssCast(matrixType, regExpr, node->getLine(), declRowSize, declColSize);
    }

    symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), mat);
    return nullptr;
}

llvm::Value *CodeGenerator::indexMatrix(llvm::Value *mat, llvm::Value *rowIdx, llvm::Value *colIdx, bool isSlice) {

    //otherwise
    llvm::Value * slice = et->sliceMatrix(mat, rowIdx, colIdx);
    llvm::Value * ret   = it->castMatrixIndex(slice, rowIdx, colIdx, mat);

    if(not(it->isStructType(ret)) && not(isSlice))
        ret = ir->CreateLoad(ret);

    return ret;
}


llvm::Value *CodeGenerator::matrixSliceAssign(ASTNode * srcNode, IndexNode * idxExpr, llvm::Value *dest, std::vector<llvm::Value *> * idxVec, llvm::Value *src) {
    llvm::Value * rowIdx    = visit(idxExpr->getIndexExpr()->at(0));
    llvm::Value * colIdx    = visit(idxExpr->getIndexExpr()->at(1));
    llvm::Value * size      = nullptr;
    llvm::Value * extraSize = nullptr;

    if(not(rowIdx->getType()->isPointerTy()) && not(colIdx->getType()->isPointerTy()))
        return indexAssign(srcNode, idxExpr, src, dest, srcNode->getLine());

    //cover null and identity cases
    if(dynamic_cast<NullNode *>(srcNode)){
        src = visit(idxExpr);
        setNullVecOrMat(src);
    }
    else if(dynamic_cast<IdnNode *>(srcNode)){
        src = visit(idxExpr);
        setIdentityVecOrMat(src);
    }

    //make scalars vectors with one element
    if(not(rowIdx->getType()->isPointerTy())){
        size = it->getValFromStruct(colIdx, VEC_LEN_INDEX);
        src  = ct->typeAssCast(it->getMatrixVectorType(dest)->getPointerElementType(), src, srcNode->getLine(), size);
        return et->assignScalarVector(dest, rowIdx, colIdx, src);
    }
    else if(not(colIdx->getType()->isPointerTy())){
        size = it->getValFromStruct(rowIdx, VEC_LEN_INDEX);
        src  = ct->typeAssCast(it->getMatrixVectorType(dest)->getPointerElementType(), src, srcNode->getLine(), size);
        return et->assignVectorScalar(dest, rowIdx, colIdx, src);
    }
    else {
        size      = it->getValFromStruct(rowIdx, VEC_LEN_INDEX);
        extraSize = it->getValFromStruct(colIdx, VEC_LEN_INDEX);
        src       = ct->typeAssCast(dest->getType()->getPointerElementType(), src, srcNode->getLine(), size, extraSize);
        return et->assignVectorVector(dest, rowIdx, colIdx, src);
    }
}

llvm::Value *CodeGenerator::visit(RowsNode *node) {
    llvm::Value *mat = visit(node->getExpr());
    return it->getMatrixNumRows(mat);
}

llvm::Value *CodeGenerator::visit(ColsNode *node) {
    llvm::Value *mat = visit(node->getExpr());
    return it->getMatrixNumCols(mat);
}
