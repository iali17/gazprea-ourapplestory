//
// Created by lepoidev on 11/18/18.
//
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <ExternalTools/ExternalTools.h>
#include <InternalTools/CondBuilder.h>

#define GET_EMPTY_VECTOR     "getEmptyVector"
#define INIT_VECTOR          "initVector"
#define SET_NULL_VECTOR      "setNullVector"
#define SET_IDENTITY_VECTOR  "setIdentityVector"
#define GET_VECTOR_LENGTH    "getVectorLength"
#define GET_REVERSE_VECTOR   "getReverseVector"
#define PRINT_VECTOR         "printVector"
#define PRINT_VECTOR_ELEMENT "printVectorElement"

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


void ExternalTools::registerVectorFunctions() {
    //getEmptyVector
    llvm::FunctionType *fTy = llvm::TypeBuilder<void* (int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_EMPTY_VECTOR, fTy));

    //initVector
    fTy = llvm::TypeBuilder<void (void *, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(INIT_VECTOR, fTy));

    //setNullVector
    fTy = llvm::TypeBuilder<void (void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(SET_NULL_VECTOR, fTy));

    //setIdentityVector
    fTy = llvm::TypeBuilder<void (void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(SET_IDENTITY_VECTOR, fTy));

    //getVectorLength
    fTy = llvm::TypeBuilder<int(void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_VECTOR_LENGTH, fTy));

    //getReverseVector
    fTy = llvm::TypeBuilder<void *(void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_REVERSE_VECTOR, fTy));

    //printVector
    fTy = llvm::TypeBuilder<void (void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(PRINT_VECTOR, fTy));

    //printVectorElement
    fTy = llvm::TypeBuilder<void (void*, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(PRINT_VECTOR_ELEMENT, fTy));
}

/**
 * Get a pointer to a new vector, size is determined on init
 * @param ty
 * @return
 */
llvm::Value *ExternalTools::getNewVector(llvm::Value *ty) {
    llvm::Function *getV = mod->getFunction(GET_EMPTY_VECTOR);
    llvm::Value    *ret  = ir->CreateCall(getV, {ty});
    return ir->CreatePointerCast(ret, vecTy->getPointerTo());
}

/**
 * Initialize a vector with a size
 * @param vec
 * @return
 */
llvm::Value *ExternalTools::initVector(llvm::Value *vec, llvm::Value *size) {
    llvm::Function *getV  = mod->getFunction(INIT_VECTOR);
    llvm::Value    *v_vec = ir->CreatePointerCast(vec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_vec, size});
    return ret;
}

/**
 * Set the passed vector to have null values
 * @param vec
 * @return
 */
llvm::Value *ExternalTools::setNullVector(llvm::Value *vec) {
    llvm::Function *getV  = mod->getFunction(SET_NULL_VECTOR);
    llvm::Value    *v_vec = ir->CreatePointerCast(vec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_vec});
    return ret;
}

/**
 * Set the passed vector to have identity
 * @param vec
 * @return
 */
llvm::Value *ExternalTools::setIdentityVector(llvm::Value *vec) {
    llvm::Function *getV  = mod->getFunction(SET_IDENTITY_VECTOR);
    llvm::Value    *v_vec = ir->CreatePointerCast(vec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_vec});
    return ret;
}

/**
 * return the length of a vector
 * @param vec
 * @return
 */
llvm::Value *ExternalTools::getVectorLength(llvm::Value *vec) {
    llvm::Function *getV  = mod->getFunction(GET_VECTOR_LENGTH);
    llvm::Value    *v_vec = ir->CreatePointerCast(vec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_vec});
    return ret;
}

/**
 * Get the reverse of a vector
 * @param fromVec
 * @return
 */
llvm::Value *ExternalTools::getReverseVector(llvm::Value *fromVec) {
    llvm::Function *getV  = mod->getFunction(GET_REVERSE_VECTOR);
    llvm::Value    *v_vec = ir->CreatePointerCast(fromVec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_vec});
    return ir->CreatePointerCast(ret, vecTy->getPointerTo());
}

/**
 * print all of the elements of a vector
 * @param v_vector
 * @return
 */
llvm::Value *ExternalTools::printVector(llvm::Value *vec) {
    llvm::Function *getV  = mod->getFunction(PRINT_VECTOR);
    llvm::Value    *v_vec = ir->CreatePointerCast(vec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_vec});
    return ret;
}
/**
 * Take a pointer to a vector elements and print the value
 * @param v_elm_ptr
 * @return
 */
llvm::Value *ExternalTools::printVectorElement(llvm::Value *vecElmPtr, llvm::Value *type) {
    llvm::Function *getV  = mod->getFunction(PRINT_VECTOR_ELEMENT);
    llvm::Value    *v_vec_elm_ptr = ir->CreatePointerCast(vecElmPtr, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_vec_elm_ptr, type});
    return ret;
}