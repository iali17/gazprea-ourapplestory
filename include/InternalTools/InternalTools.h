//
// Created by kyle on 20/10/18.
//

#ifndef VCALCBASE_INTERNALTOOLS_H
#define VCALCBASE_INTERNALTOOLS_H

#include <vector>
#include <string>
#include <iostream>
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
	llvm::Value *getValFromTuple(llvm::Value *tuplePtr, llvm::Value *idx);
	llvm::Value *getPtrFromTuple(llvm::Value *tuplePtr, llvm::Value *idx);
	llvm::Value *initTuple(llvm::Value *tuplePtr, std::vector<llvm::Value *> *values);
    llvm::Value *geti1(int64_t val);
    llvm::Value *geti8(int64_t val);
    llvm::Value *getReal(float val);
    llvm::Value *getConsi32(int64_t val);
    llvm::Value *getConsi64(int64_t val);
    llvm::Value *safeReplace(llvm::Value *safePtr, llvm::Value *cond, llvm::Value *idx,
                             llvm::Value *arrP, llvm::Value *replaceVal);
    llvm::Value *getNull(llvm::Type *type);
    llvm::Value *getIdn(llvm::Type *type);
    llvm::Value *getAdd(llvm::Value *left, llvm::Value * right);
    llvm::Value *getSub(llvm::Value *left, llvm::Value * right);
    llvm::Value *getMul(llvm::Value *left, llvm::Value * right);
    llvm::Value *getDiv(llvm::Value *left, llvm::Value * right);
	llvm::Value *getRem(llvm::Value *left, llvm::Value * right);
    llvm::Value *getEQ (llvm::Value *left, llvm::Value * right);
    llvm::Value *getNEQ(llvm::Value *left, llvm::Value * right);
    llvm::Value *getGT (llvm::Value *left, llvm::Value * right);
    llvm::Value *getLT (llvm::Value *left, llvm::Value * right);
    llvm::Value *getLTE(llvm::Value *left, llvm::Value * right);
    llvm::Value *getGTE(llvm::Value *left, llvm::Value * right);
    llvm::Value *getAnd(llvm::Value *left, llvm::Value * right);
    llvm::Value *getOr (llvm::Value *left, llvm::Value * right);
    llvm::Value *getXOr(llvm::Value *left, llvm::Value * right);
    llvm::Value *getNegation(llvm::Value *expr);

    pair makePair(llvm::Value *left, llvm::Value *right);
    bool setNull(llvm::Type * type, llvm::Value * ptr);
    bool isStructType(llvm::Value *ptr);
    std::vector<llvm::Value *> * getValueVectorFromStruct(llvm::Value * structPtr);

protected:
    llvm::LLVMContext * globalCtx;
    llvm::IRBuilder<> * ir;
    llvm::Module      * mod;
};

#endif //VCALCBASE_INTERNALTOOLS_H
