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

/**
 * set up all interval functions
 */
void ExternalTools::registerIntervalFunctions() {
    llvm::FunctionType *fTy = llvm::TypeBuilder<void* (int, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("getNewInterval", fTy));

    //initVector
    fTy = llvm::TypeBuilder<void* (void *, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("getVectorFromInterval", fTy));
}

/**
 * Ger a new interval with the given bounds
 * @param lower
 * @param upper
 * @return
 */
llvm::Value *ExternalTools::getNewInterval(llvm::Value *lower, llvm::Value *upper) {
    llvm::Function *getV = mod->getFunction("getNewInterval");
    llvm::Value    *ret  = ir->CreateCall(getV, {lower, upper});
    return ir->CreatePointerCast(ret, intervalTy->getPointerTo());
}

/**
 * Get a vector from the invertval given the frequency 'by'
 * @param interval
 * @param by
 * @return
 */
llvm::Value *ExternalTools::getVectorFromInterval(llvm::Value *interval, llvm::Value *by) {
    llvm::Function *getV  = mod->getFunction("getVectorFromInterval");
    llvm::Value    *v_int = ir->CreatePointerCast(interval, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_int, by});
    return ir->CreatePointerCast(ret, vecTy->getPointerTo());
}
