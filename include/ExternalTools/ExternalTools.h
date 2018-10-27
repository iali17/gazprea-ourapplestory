//
// Created by kyle on 20/10/18.
//

#ifndef VCALCBASE_EXTERNALTOOLS_H
#define VCALCBASE_EXTERNALTOOLS_H

#include <vector>
#include <string>
#include "llvm/IR/Value.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/IR/Verifier.h"
#include "InternalTools/InternalTools.h"

#define INTFORMAT_STR   "intFormatStr"
#define CHARFORMAT_STR  "charFormatStr"
#define FLOATFORMAT_STR "floatFormatStr"
#define EOLN_STR        "eolnStr"
#define SPACE_STR       "spaceStr"
#define OPENSQRB_STR    "openSqrBStr"
#define CLOSESQRB_STR   "closeSqrBStr"

class ExternalTools {
public:
    ExternalTools(llvm::LLVMContext *globalCtx, llvm::IRBuilder<> *ir, llvm::Module *mod);

    //calloc
    void registerCalloc();
    llvm::Value * aliCalloc(llvm::Value * arrSize, int elementSize = 4, bool castToIntP = true);

    //free
    void registerFree();
    void aliFree(llvm::Value * p);

    //scanf
    void registerScanf();
    llvm::Value *aliScanf(std::string consScanString, llvm::Value *scanTo);
    llvm::Value *aliScanf(llvm::Value *scanTo);

    //printing
    void registerPrintf();
    void printStaticStr(std::string constStr);
    void printInt(llvm::Value * val);
    void printReal(llvm::Value * val);
    void printStr(llvm::Value *val);
    void printChar(llvm::Value *val);
    void printBoolean(llvm::Value *val);
    void print(llvm::Value * val);

protected:
    llvm::LLVMContext * globalCtx;
    llvm::IRBuilder<> * ir;
    llvm::Module      * mod;
    //TODO - move these declarations
};
#endif //VCALCBASE_EXTERNALTOOLS_H
