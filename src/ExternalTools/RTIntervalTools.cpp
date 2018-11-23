//
// Created by lepoidev on 11/18/18.
//

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <ExternalTools/ExternalTools.h>
#include <InternalTools/CondBuilder.h>

#define GET_NEW_INTERVAL "getNewInterval"
#define GET_VECTOR_FROM_INTERVAL "getVectorFromInterval"

extern llvm::Type *charTy;
extern llvm::Type *intVecTy;
extern llvm::Type *intervalTy;

/**
 * set up all interval functions
 */
void ExternalTools::registerIntervalFunctions() {
    llvm::FunctionType *fTy = llvm::TypeBuilder<void* (int, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_NEW_INTERVAL, fTy));

    //initVector
    fTy = llvm::TypeBuilder<void* (void *, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction(GET_VECTOR_FROM_INTERVAL, fTy));
}

/**
 * Get a new interval with the given bounds
 * @param lower
 * @param upper
 * @return
 */
llvm::Value *ExternalTools::getNewInterval(llvm::Value *lower, llvm::Value *upper) {
    llvm::Function *getV = mod->getFunction(GET_NEW_INTERVAL);
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
    llvm::Function *getV  = mod->getFunction(GET_VECTOR_FROM_INTERVAL);
    llvm::Value    *v_int = ir->CreatePointerCast(interval, charTy->getPointerTo());
    llvm::Value    *ret   = ir->CreateCall(getV, {v_int, by});
    return ir->CreatePointerCast(ret, intVecTy->getPointerTo());
}
