//
// Created by lepoidev on 11/18/18.
//
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <ExternalTools/ExternalTools.h>
#include <InternalTools/CondBuilder.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;
extern llvm::Type *vecTy;
extern llvm::Type *matrixTy;
extern llvm::Type *intervalTy;


void ExternalTools::registerVectorFunctions() {
    //getEmptyVector
    llvm::FunctionType *fTy = llvm::TypeBuilder<void* (int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("getEmptyVector", fTy));

    //initVector
    fTy = llvm::TypeBuilder<void (void *, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("initVector", fTy));

    //setNullVector
    fTy = llvm::TypeBuilder<void (void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("setNullVector", fTy));

    //setIdentityVector
    fTy = llvm::TypeBuilder<void (void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("setIdentityVector", fTy));

    //getVectorLength
    fTy = llvm::TypeBuilder<int(void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("getVectorLength", fTy));

    //getReverseVector
    fTy = llvm::TypeBuilder<void *(void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("getReverseVector", fTy));

    //printVector
    fTy = llvm::TypeBuilder<void (void*), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("printVector", fTy));

    //printVectorElement
    fTy = llvm::TypeBuilder<void (void*, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("printVectorElement", fTy));
}

/**
 * Get a pointer to a new vector, size is determined on init
 * @param ty
 * @return
 */
llvm::Value *ExternalTools::getNewVector(llvm::Value *ty) {
    llvm::Function *getV = mod->getFunction("getEmptyVector");
    llvm::Value    *ret  = ir->CreateCall(getV, {ty});
    return ir->CreatePointerCast(ret, vecTy->getPointerTo());
}

/**
 * Initialize a vector with a size
 * @param vec
 * @return
 */
llvm::Value *ExternalTools::initVector(llvm::Value *vec, llvm::Value *size) {
    llvm::Function *getV  = mod->getFunction("initVector");
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
    llvm::Function *getV  = mod->getFunction("setNullVector");
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
    llvm::Function *getV  = mod->getFunction("setIdentityVector");
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
    llvm::Function *getV  = mod->getFunction("getVectorLength");
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
    llvm::Function *getV  = mod->getFunction("getReverseVector");
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
    llvm::Function *getV  = mod->getFunction("printVector");
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
    llvm::Function *getV  = mod->getFunction("printVectorElement");
    llvm::Value    *v_vec_elm_ptr = ir->CreatePointerCast(vecElmPtr, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_vec_elm_ptr, type});
    return ret;
}
