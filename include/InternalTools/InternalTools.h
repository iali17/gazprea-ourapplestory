//
// Created by kyle on 20/10/18.
//

#ifndef VCALCBASE_INTERNALTOOLS_H
#define VCALCBASE_INTERNALTOOLS_H

#include <vector>
#include <string>
#include "AST/ASTNodes/BaseNodes/ASTNode.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/IR/Verifier.h"
#include "globals.h"

class InternalTools {
public:
    struct pair{
        pair():left(nullptr),right(nullptr){};
        llvm::Value *left;
        llvm::Value *right;
    };

    InternalTools(llvm::LLVMContext *globalCtx, llvm::IRBuilder<> *ir, llvm::Module *mod);
    void setUpTypes();
    //tools
    llvm::Value *geti1(int64_t val);
    llvm::Value *geti8(int64_t val);
    llvm::Value *getReal(float val);
    llvm::Value *getConsi32(int64_t val);
    llvm::Value *getConsi64(int64_t val);
    llvm::Value *safeReplace(llvm::Value *safePtr, llvm::Value *cond, llvm::Value *idx,
                             llvm::Value *arrP, llvm::Value *replaceVal);
    llvm::Value *getNull(llvm::Type *type);
    llvm::Value *getIdn(llvm::Type *type);
    pair makePair(llvm::Value *left, llvm::Value *right);
    bool setNull(llvm::Type * type, llvm::Value * ptr);


protected:
    llvm::LLVMContext * globalCtx;
    llvm::IRBuilder<> * ir;
    llvm::Module      * mod;
};

#endif //VCALCBASE_INTERNALTOOLS_H