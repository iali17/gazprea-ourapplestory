//
// Created by lepoidev on 11/20/18.
//

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <ExternalTools/ExternalTools.h>
#include <InternalTools/CondBuilder.h>

//local constants for function names
#define GET_EMPTY_MATRIX    "getEmptyMatrix"
#define INIT_MATRIX         "initMatrix"
#define SET_NULL_MATRIX     "setNullMatrix"
#define SET_IDENTITY_MATRIX "setIdentityMatrix"
#define GET_NUM_ROWS        "getNumRows"
#define GET_NUM_COLS        "getNumCols"

extern llvm::Type *charTy;

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
}

llvm::Value *ExternalTools::getNewMatrix(llvm::Value *ty) {
    llvm::Function *getM  = mod->getFunction(GET_EMPTY_MATRIX);
    llvm::Value    *ret   = ir->CreateCall(getM, {ty});
    return ret;
}

llvm::Value *ExternalTools::initMatrix(llvm::Value *mat, llvm::Value *numRows, llvm::Value *numCols) {
    llvm::Function *getM  = mod->getFunction(INIT_MATRIX);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat, numRows, numCols});
    return ret;
}

llvm::Value *ExternalTools::setNullMatrix(llvm::Value *mat) {
    llvm::Function *getM  = mod->getFunction(SET_NULL_MATRIX);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat});
    return ret;
}

llvm::Value *ExternalTools::setIdentityMatrix(llvm::Value *mat) {
    llvm::Function *getM  = mod->getFunction(SET_IDENTITY_MATRIX);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat});
    return ret;
}

llvm::Value *ExternalTools::getNumRows(llvm::Value *mat) {
    llvm::Function *getM  = mod->getFunction(GET_NUM_ROWS);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat});
    return ret;
}

llvm::Value *ExternalTools::getNumCols(llvm::Value *mat) {
    llvm::Function *getM  = mod->getFunction(GET_NUM_COLS);
    llvm::Value    *v_mat = ir->CreatePointerCast(mat, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getM, {v_mat});
    return ret;
}
