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
}

llvm::Value *ExternalTools::getNewVector(llvm::Value *ty) {
    llvm::Function *getV = mod->getFunction("getEmptyVector");
    llvm::Value    *ret = ir->CreateCall(getV, {ty});
    return ir->CreatePointerCast(ret, vecTy->getPointerTo());
}

llvm::Value *ExternalTools::initVector(llvm::Value *vec) {
    llvm::Function *getV = mod->getFunction("initVector");
    llvm::Value    *ret = ir->CreateCall(getV, {vec});
    return ret;
}

llvm::Value *ExternalTools::setNullVector(llvm::Value *vec) {
    llvm::Function *getV = mod->getFunction("setNullVector");
    llvm::Value    *ret = ir->CreateCall(getV, {vec});
    return ret;
}

llvm::Value *ExternalTools::setIdentityVector(llvm::Value *vec) {
    llvm::Function *getV = mod->getFunction("setIdentityVector");
    llvm::Value    *ret = ir->CreateCall(getV, {vec});
    return ret;
}
