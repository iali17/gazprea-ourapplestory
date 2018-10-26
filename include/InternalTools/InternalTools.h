//
// Created by kyle on 20/10/18.
//

#ifndef VCALCBASE_INTERNALTOOLS_H
#define VCALCBASE_INTERNALTOOLS_H

#include <vector>
#include <string>
#include "llvm/IR/Value.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/IR/Verifier.h"
#include "globals.h"

class InternalTools {
public:
    InternalTools(llvm::LLVMContext *globalCtx, llvm::IRBuilder<> *ir, llvm::Module *mod);
    void setUpTypes();
    //tools
    llvm::Value *geti8(int64_t val);
    llvm::Value *getReal(double val);
    llvm::Value *getConsi32(int64_t val);
    llvm::Value *getConsi64(int64_t val);
    llvm::Value *safeReplace(llvm::Value *safePtr, llvm::Value *cond, llvm::Value *idx,
                             llvm::Value *arrP, llvm::Value *replaceVal);
    llvm::Type *getTypeFromString(std::string strType);
protected:
    llvm::LLVMContext * globalCtx;
    llvm::IRBuilder<> * ir;
    llvm::Module      * mod;
};
#endif //VCALCBASE_INTERNALTOOLS_H