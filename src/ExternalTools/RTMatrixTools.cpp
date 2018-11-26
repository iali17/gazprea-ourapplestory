//
// Created by lepoidev on 11/20/18.
//

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <ExternalTools/ExternalTools.h>
#include <InternalTools/CondBuilder.h>

//local constants for function names
#define GET_EMPTY_MATRIX     "getEmptyMatrix"
#define INIT_MATRIX          "initMatrix"
#define SET_NULL_MATRIX      "setNullMatrix"
#define SET_IDENTITY_MATRIX  "setIdentityMatrix"
#define GET_NUM_ROWS         "getNumRows"
#define GET_NUM_COLS         "getNumCols"
#define INDEX_SCALAR_VECTOR  "indexScalarVector"
#define INDEX_VECTOR_SCALAR  "indexVectorScalar"
#define INDEX_VECTOR_VECTOR  "indexVectorVector"
#define PRINT_MATRIX         "printMatrix"
#define SLICE_SCALAR_VECTOR  "sliceScalarVector"
#define SLICE_VECTOR_SCALAR  "sliceVectorScalar"
#define SLICE_VECTOR_VECTOR  "sliceVectorVector"
#define MATRIX_GEP           "getMatrixElementPointer"
#define INT_MATRIX_MULT      "getIntMatrixMultiplication"
#define REAL_MATRIX_MULT     "getRealMatrixMultiplication"
#define CONCATENATE_MATRICES "concatenateMatrices"

extern llvm::Type *charTy;
extern llvm::Type *intMatrixTy;
extern llvm::Type *realMatrixTy;

void ExternalTools::registerMatrixFunctions() {
    llvm::FunctionType *fTy = llvm::TypeBuilder<void* (void), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_EMPTY_MATRIX, fTy));

    //initMatrix
    fTy = llvm::TypeBuilder<void (void *, int, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(INIT_MATRIX, fTy));

    //setNullMatrix
    fTy = llvm::TypeBuilder<void (void *), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(SET_NULL_MATRIX, fTy));

    //setIdentityMatrix
    fTy = llvm::TypeBuilder<void (void *), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(SET_IDENTITY_MATRIX, fTy));

    //setNumCols
    fTy = llvm::TypeBuilder<int (void *), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_NUM_COLS, fTy));

    //setNumRows
    fTy = llvm::TypeBuilder<int (void *), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_NUM_ROWS, fTy));

    //indexScalarVector
    fTy = llvm::TypeBuilder<void* (int, void *), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(INDEX_SCALAR_VECTOR, fTy));

    //indexVectorScalar
    fTy = llvm::TypeBuilder<void* (void *, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(INDEX_VECTOR_SCALAR, fTy));

    //indexVectorVector
    fTy = llvm::TypeBuilder<void* (void *, void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(INDEX_VECTOR_VECTOR, fTy));

    //printMatrix
    fTy = llvm::TypeBuilder<void (void *), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(PRINT_MATRIX, fTy));

    //sliceScalarVector
    fTy = llvm::TypeBuilder<void* (int, void *), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(SLICE_SCALAR_VECTOR, fTy));

    //sliceVectorScalar
    fTy = llvm::TypeBuilder<void* (void *, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(SLICE_VECTOR_SCALAR, fTy));

    //sliceVectorVector
    fTy = llvm::TypeBuilder<void* (void *, void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(SLICE_VECTOR_VECTOR, fTy));

    //getMatrixElementPointer
    fTy = llvm::TypeBuilder<void* (void *, int, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(MATRIX_GEP, fTy));

    //getIntMatrixMultiplication
    fTy = llvm::TypeBuilder<void* (void *, void *), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(INT_MATRIX_MULT, fTy));

    //getRealMatrixMultiplication
    fTy = llvm::TypeBuilder<void* (void *, void *), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(REAL_MATRIX_MULT, fTy));

    //concatenateMatrices
    fTy = llvm::TypeBuilder<void* (void *, void *), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(CONCATENATE_MATRICES, fTy));
}

/**
 * Allocate space for a new matrix of the given type
 * @param ty
 * @return
 */
llvm::Value *ExternalTools::getNewMatrix(llvm::Value *ty) {
    llvm::Function *getM  = mod->getFunction(GET_EMPTY_MATRIX);
    llvm::Value    *ret   = ir->CreateCall(getM, {ty});
    return ret;
}

/**
 * Initialize the given matrix with a given size. init to proper null values
 * @param mat
 * @param numRows
 * @param numCols
 * @return
 */
llvm::Value *ExternalTools::initMatrix(llvm::Value *mat, llvm::Value *numRows, llvm::Value *numCols) {
    llvm::Function *getM  = mod->getFunction(INIT_MATRIX);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat, numRows, numCols});
    return ret;
}

/**
 * Sets the given matrix to give all elements their proper null values
 * @param mat
 * @return
 */
llvm::Value *ExternalTools::setNullMatrix(llvm::Value *mat) {
    llvm::Function *getM  = mod->getFunction(SET_NULL_MATRIX);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat});
    return ret;
}

/**
 * Sets the given matrix to give all elements their proper identity values
 * @param mat
 * @return
 */
llvm::Value *ExternalTools::setIdentityMatrix(llvm::Value *mat) {
    llvm::Function *getM  = mod->getFunction(SET_IDENTITY_MATRIX);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat});
    return ret;
}

/**
 * retuns the number of rows of a given matrix
 * @param mat
 * @return
 */
llvm::Value *ExternalTools::getNumRows(llvm::Value *mat) {
    llvm::Function *getM  = mod->getFunction(GET_NUM_ROWS);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat});
    return ret;
}

/**
 * returns the number of cols of a given matrix
 * @param mat
 * @return
 */
llvm::Value *ExternalTools::getNumCols(llvm::Value *mat) {
    llvm::Function *getM  = mod->getFunction(GET_NUM_COLS);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat});
    return ret;
}

/**
 * prints the matrix
 * @param mat
 * @return
 */
llvm::Value *ExternalTools::printMatrix(llvm::Value *mat) {
    llvm::Function *getM  = mod->getFunction(PRINT_MATRIX);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat});
    return ret;
}

/**
 * general slice call. just calls the other functions
 * @param mat
 * @param l
 * @param r
 * @return
 */
llvm::Value *ExternalTools::sliceMatrix(llvm::Value *mat, llvm::Value *l, llvm::Value *r) {
    if(l->getType()->isPointerTy() && r->getType()->isPointerTy())
        return sliceVectorVector(mat, l, r);
    else if(l->getType()->isPointerTy())
        return sliceVectorScalar(mat, l, r);
    else if(r->getType()->isPointerTy())
        return sliceScalarVector(mat, l, r);
    else
        return nullptr;
}

/**
 * slices a vector with index scalar,vector. you gotta cast the vector type on the other side
 * @param mat
 * @param scalar
 * @param vec
 * @return
 */
llvm::Value *ExternalTools::sliceScalarVector(llvm::Value *mat, llvm::Value *scalar, llvm::Value *vec) {
    llvm::Function *getM  = mod->getFunction(SLICE_SCALAR_VECTOR);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *v_vec = ir->CreatePointerCast(vec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat, scalar, v_vec});
    return ret;
}

/**
 * slices a vector with index vector, scalar. you gotta cast the vector type on the other side
 * @param mat
 * @param vec
 * @param scalar
 * @return
 */
llvm::Value *ExternalTools::sliceVectorScalar(llvm::Value *mat, llvm::Value *vec, llvm::Value *scalar) {
    llvm::Function *getM  = mod->getFunction(SLICE_VECTOR_SCALAR);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *v_vec = ir->CreatePointerCast(vec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat, v_vec, scalar});
    return ret;
}

/**
 * slices a vector with index vector,vector. ill cast this one
 * @param mat
 * @param vecRow
 * @param vecCol
 * @return
 */
llvm::Value *ExternalTools::sliceVectorVector(llvm::Value *mat, llvm::Value *vecRow, llvm::Value *vecCol) {
    llvm::Function *getM      = mod->getFunction(SLICE_VECTOR_VECTOR);
    llvm::Value    *v_mat     = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *v_vec_row = ir->CreatePointerCast(vecRow, charTy->getPointerTo());
    llvm::Value    *v_vec_col = ir->CreatePointerCast(vecCol, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat, v_vec_row, v_vec_col});
    return ir->CreatePointerCast(ret, mat->getType());
}

/**
 * general index call. just calls the other functions
 * @param mat
 * @param l
 * @param r
 * @return
 */
llvm::Value *ExternalTools::indexMatrix(llvm::Value *mat, llvm::Value *l, llvm::Value *r) {
    if(l->getType()->isPointerTy() && r->getType()->isPointerTy())
        return indexVectorVector(mat, l, r);
    else if(l->getType()->isPointerTy())
        return indexVectorScalar(mat, l, r);
    else if(r->getType()->isPointerTy())
        return indexScalarVector(mat, l, r);
    else
        return nullptr;
}

/**
 * indexes a matrix by scalar, vector. return is not casted
 * @param mat
 * @param scalar
 * @param vec
 * @return
 */
llvm::Value *ExternalTools::indexScalarVector(llvm::Value *mat, llvm::Value *scalar, llvm::Value *vec) {
    llvm::Function *getM  = mod->getFunction(INDEX_SCALAR_VECTOR);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *v_vec = ir->CreatePointerCast(vec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat, scalar, v_vec});
    return ret;
}

/**
 * indexes a matrix by vector, scalar. return is not casted
 * @param mat
 * @param vec
 * @param scalar
 * @return
 */
llvm::Value *ExternalTools::indexVectorScalar(llvm::Value *mat, llvm::Value *vec, llvm::Value *scalar) {
    llvm::Function *getM  = mod->getFunction(INDEX_VECTOR_SCALAR);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *v_vec = ir->CreatePointerCast(vec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat, v_vec, scalar});
    return ret;
}

/**
 * indexes a matrix by two vectors. return is casted
 * @param mat
 * @param vecRow
 * @param vecCol
 * @return
 */
llvm::Value *ExternalTools::indexVectorVector(llvm::Value *mat, llvm::Value *vecRow, llvm::Value *vecCol) {
    llvm::Function *getM      = mod->getFunction(INDEX_VECTOR_VECTOR);
    llvm::Value    *v_mat     = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *v_vec_row = ir->CreatePointerCast(vecRow, charTy->getPointerTo());
    llvm::Value    *v_vec_col = ir->CreatePointerCast(vecCol, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat, v_vec_row, v_vec_col});
    return ir->CreatePointerCast(ret, mat->getType());
}

/**
 * returns a pointer to a matrix element. it will return a void pointer. it is best that use a IT function
 * so that you don't have to cast the return type
 * @param mat
 * @param row
 * @param col
 * @return
 */
llvm::Value *ExternalTools::getMatrixElementPointer(llvm::Value *mat, llvm::Value *row, llvm::Value *col) {
    llvm::Function *getM  = mod->getFunction(MATRIX_GEP);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat, row, col});
    return ret;
}

/**
 * Assumes that both matrices have beeeeeeeen type casted already
 * @param leftMat
 * @param rightMat
 * @return
 */
llvm::Value *ExternalTools::getMatrixMultiplication(llvm::Value *leftMat, llvm::Value *rightMat) {
    if(leftMat->getType() == intMatrixTy->getPointerTo())
        return getIntMatrixMultiplication(leftMat, rightMat);
    else if(leftMat->getType() == realMatrixTy->getPointerTo())
        return getRealMatrixMultiplication(leftMat, rightMat);
    return nullptr;
}

/**
 * Performs the integer matrix multiplication. I cast for you
 * @param leftMat
 * @param rightMat
 * @return
 */
llvm::Value *ExternalTools::getIntMatrixMultiplication(llvm::Value *leftMat, llvm::Value *rightMat) {
    llvm::Function *getM    = mod->getFunction(INT_MATRIX_MULT);
    llvm::Value    *v_left  = ir->CreatePointerCast(leftMat, charTy->getPointerTo());
    llvm::Value    *v_right = ir->CreatePointerCast(rightMat, charTy->getPointerTo());
    llvm::Value    *ret     = ir->CreateCall(getM, {v_left, v_right});
    return ir->CreatePointerCast(ret, leftMat->getType());
}

/**
 * Performs the real matrix multiplication. I cast for you
 * @param leftMat
 * @param rightMat
 * @return
 */
llvm::Value *ExternalTools::getRealMatrixMultiplication(llvm::Value *leftMat, llvm::Value *rightMat) {
    llvm::Function *getM    = mod->getFunction(REAL_MATRIX_MULT);
    llvm::Value    *v_left  = ir->CreatePointerCast(leftMat, charTy->getPointerTo());
    llvm::Value    *v_right = ir->CreatePointerCast(rightMat, charTy->getPointerTo());
    llvm::Value    *ret     = ir->CreateCall(getM, {v_left, v_right});
    return ir->CreatePointerCast(ret, leftMat->getType());
}

/**
 * concatenate matrices and cast return
 * @param left
 * @param right
 * @return
 */
llvm::Value *ExternalTools::concatenateMatrices(llvm::Value *left, llvm::Value *right) {
    llvm::Function *getM    = mod->getFunction(CONCATENATE_MATRICES);
    llvm::Value    *v_left  = ir->CreatePointerCast(left, charTy->getPointerTo());
    llvm::Value    *v_right = ir->CreatePointerCast(right, charTy->getPointerTo());
    llvm::Value    *ret     = ir->CreateCall(getM, {v_left, v_right});
    return ir->CreatePointerCast(ret, left->getType());
}
