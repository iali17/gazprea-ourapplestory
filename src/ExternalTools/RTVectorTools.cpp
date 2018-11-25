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
#define COPY_VECTOR_ELEMENTS "copyVectorElements"
#define STRICT_COPY_VECTOR   "strictCopyVectorElements"
#define GET_VECTOR_SLICE     "getVectorSlice"
#define GET_VECTOR_COPY      "getVectorCopy"
#define VECTOR_GEP           "getVectorElementPointer"
#define GET_INT_DOT_PRODUCT  "getIntDotProduct"
#define GET_REAL_DOT_PRODUCT "getRealDotProduct"
#define ASSIGN_VAL_FROM_PTRS "assignValFromPointers"
#define GET_OP_RESULT_VECTOR "getOpResultVector"

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

    //copyVectorElements
    fTy = llvm::TypeBuilder<void (void*, void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(COPY_VECTOR_ELEMENTS, fTy));

    //strictCopyVectorElements
    fTy = llvm::TypeBuilder<void (void*, void*, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(STRICT_COPY_VECTOR, fTy));

    //getVectorSlice
    fTy = llvm::TypeBuilder<void* (void*, void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_VECTOR_SLICE, fTy));

    //getVectorCopy
    fTy = llvm::TypeBuilder<void* (void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_VECTOR_COPY, fTy));

    //getVectorElementPointer
    fTy = llvm::TypeBuilder<void* (void*, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(VECTOR_GEP, fTy));

    //getIntDotProduct
    fTy = llvm::TypeBuilder<int (void*, void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_INT_DOT_PRODUCT, fTy));

    //getRealDotProduct
    fTy = llvm::TypeBuilder<float (void*, void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_REAL_DOT_PRODUCT, fTy));

    //assignValFromPointers
    fTy = llvm::TypeBuilder<void (void*, void*, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(ASSIGN_VAL_FROM_PTRS, fTy));

    //getOpResultVector
    fTy = llvm::TypeBuilder<void* (void*, void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_OP_RESULT_VECTOR, fTy));
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

/**
 * copies the elements of src into the elements of dest. padding performed
 * @param dest
 * @param src
 * @return
 */
llvm::Value *ExternalTools::copyVectorElements(llvm::Value *dest, llvm::Value *src) {
    llvm::Function *getV   = mod->getFunction(COPY_VECTOR_ELEMENTS);
    llvm::Value    *v_dest = ir->CreatePointerCast(dest, charTy->getPointerTo());
    llvm::Value    *v_src  = ir->CreatePointerCast(src, charTy->getPointerTo());
    return ir->CreateCall(getV, {v_dest, v_src});
}

/**
 * copies the elements of src into the elements of dest, only if size is the same
 * otherwise, throw error
 * @param dest
 * @param src
 * @param line
 * @return
 */
llvm::Value *ExternalTools::strictCopyVectorElements(llvm::Value *dest, llvm::Value *src, llvm::Value *line) {
    llvm::Function *getV   = mod->getFunction(STRICT_COPY_VECTOR);
    llvm::Value    *v_dest = ir->CreatePointerCast(dest, charTy->getPointerTo());
    llvm::Value    *v_src  = ir->CreatePointerCast(src, charTy->getPointerTo());
    return ir->CreateCall(getV, {v_dest, v_src, line});
}

/**
 * Get the slice of a vector and casts it for you too
 * @param vec
 * @param idxVec
 * @return
 */
llvm::Value *ExternalTools::getVectorSlice(llvm::Value *vec, llvm::Value *idxVec) {
    llvm::Function *getV  = mod->getFunction(GET_VECTOR_SLICE);
    llvm::Value    *v_vec = ir->CreatePointerCast(vec, charTy->getPointerTo());
    llvm::Value    *v_idx = ir->CreatePointerCast(idxVec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_vec, v_idx});
    return ir->CreatePointerCast(ret, vec->getType());
}

/**
 * Create a clone of the vector and casts it for you too
 * @param vec
 * @return
 */
llvm::Value *ExternalTools::getVectorCopy(llvm::Value *vec) {
    llvm::Function *getV  = mod->getFunction(GET_VECTOR_COPY);
    llvm::Value    *v_vec = ir->CreatePointerCast(vec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_vec});
    return ir->CreatePointerCast(ret, vec->getType());
}

/**
 * Returns a pointer to a given element. you will have to cast the return type
 * @param vec
 * @param idx
 * @return
 */
llvm::Value *ExternalTools::getVectorElementPointer(llvm::Value *vec, llvm::Value *idx) {
    llvm::Function *getV  = mod->getFunction(VECTOR_GEP);
    llvm::Value    *v_vec = ir->CreatePointerCast(vec, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_vec, idx});
    return ret;
}

/**
 * performs the dot product on int vectors and returns the result
 * @param leftVec
 * @param rightVec
 * @return
 */
llvm::Value *ExternalTools::getIntDotProduct(llvm::Value *leftVec, llvm::Value *rightVec) {
    llvm::Function *getV   = mod->getFunction(GET_INT_DOT_PRODUCT);
    llvm::Value    *v_dest = ir->CreatePointerCast(leftVec,  charTy->getPointerTo());
    llvm::Value    *v_src  = ir->CreatePointerCast(rightVec, charTy->getPointerTo());
    return ir->CreateCall(getV, {v_dest, v_src});
}

/**
 * performs the dot product on real vectors and returns the result
 * @param leftVec
 * @param rightVec
 * @return
 */
llvm::Value *ExternalTools::getRealDotProduct(llvm::Value *leftVec, llvm::Value *rightVec) {
    llvm::Function *getV   = mod->getFunction(GET_REAL_DOT_PRODUCT);
    llvm::Value    *v_dest = ir->CreatePointerCast(leftVec,  charTy->getPointerTo());
    llvm::Value    *v_src  = ir->CreatePointerCast(rightVec, charTy->getPointerTo());
    return ir->CreateCall(getV, {v_dest, v_src});
}

/**
 * Checks op type and return the dot product
 * @param leftVec
 * @param rightVec
 * @return
 */
llvm::Value *ExternalTools::getDotProduct(llvm::Value *leftVec, llvm::Value *rightVec) {
    if(leftVec->getType() == intVecTy->getPointerTo())
        return getIntDotProduct(leftVec, rightVec);
    else if(leftVec->getType() == realVecTy->getPointerTo())
        return getRealDotProduct(leftVec, rightVec);
    return nullptr;
}

/**
 * Assigns right hand value to left hand value assuming the type is ty
 * @param left
 * @param right
 * @param ty
 * @return
 */
llvm::Value *ExternalTools::assignValFromPointers(llvm::Value *left, llvm::Value *right, llvm::Value *ty) {
    llvm::Function *getV    = mod->getFunction(ASSIGN_VAL_FROM_PTRS);
    llvm::Value    *v_left  = ir->CreatePointerCast(left, charTy->getPointerTo());
    llvm::Value    *v_right = ir->CreatePointerCast(right, charTy->getPointerTo());
    return ir->CreateCall(getV, {v_left, v_right, ty});
}

/**
 * allocates a new vector with the proper size for the return
 * @param left
 * @param right
 * @return
 */
llvm::Value *ExternalTools::getOpResultVector(llvm::Value *left, llvm::Value *right) {
    llvm::Function *getV    = mod->getFunction(GET_OP_RESULT_VECTOR);
    llvm::Value    *v_left  = ir->CreatePointerCast(left, charTy->getPointerTo());
    llvm::Value    *v_right = ir->CreatePointerCast(right, charTy->getPointerTo());
    llvm::Value    *ret     = ir->CreateCall(getV, {v_left, v_right});
    return ir->CreatePointerCast(ret, left->getType());
}
