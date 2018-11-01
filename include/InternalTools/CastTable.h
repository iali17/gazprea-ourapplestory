//
// Created by ali5 on 10/26/18.
//

#ifndef GAZPREABASE_CASTTABLE_H
#define GAZPREABASE_CASTTABLE_H

#include <iostream>

#include <llvm/IR/IRBuilder.h>
#include "InternalTools.h"
#include "globals.h"

class CastTable {
public:
    CastTable(llvm::LLVMContext *globalctx, llvm::IRBuilder<> *ir);

    llvm::Value *typePromotion(llvm::Value *leftExpr, llvm::Value *rightExpr);
    llvm::Value *varCast(llvm::Type *type, llvm::Value *expr);

    int getType(llvm::Type *expr);

private:
    std::string typeTable[4][4] = { {"bool", "char", "int", "real"},
                                    {"bool", "char", "int", "real"},
                                    {"bool", "char", "int", "real"},
                                    {"null", "null", "int", "real"}};

    std::string typePTable[4][4] = { {"bool", "null", "null", "null"},
                                     {"null", "char", "null", "null"},
                                     {"null", "null", "int", "real"},
                                     {"null", "null", "real", "real"}};

protected:
    llvm::LLVMContext * globalCtx;
    llvm::IRBuilder<> * ir;
};

#endif //GAZPREABASE_CASTTABLE_H
