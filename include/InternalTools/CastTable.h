//
// Created by ali5 on 10/26/18.
//

#ifndef GAZPREABASE_CASTTABLE_H
#define GAZPREABASE_CASTTABLE_H

#include <iostream>

#include <llvm/IR/IRBuilder.h>
#include "InternalTools.h"
#include "ExternalTools/ExternalTools.h"
#include "CondBuilder.h"
#include "ErrorBuilder.h"
#include "WhileBuilder.h"
#include "AST/ASTNodes/ErrorNodes/ScalarNode.h"
#include "AST/ASTNodes/TerminalNodes/VectorNode.h"
#include "globals.h"

class CastTable {
public:
    CastTable(llvm::LLVMContext *globalctx, llvm::IRBuilder<> *ir, InternalTools *it, ExternalTools *et, llvm::Module *mod, ErrorBuilder *eb);

    InternalTools::pair typePromotion(llvm::Value *leftExpr, llvm::Value *rightExpr, int line);
    llvm::Value *varCast(llvm::Type *type, llvm::Value *expr, int line);

    llvm::Value *typeAssCast(llvm::Type *type, llvm::Value *expr, int line, llvm::Value *size = nullptr);
    llvm::Value *vecAssCast(llvm::Type *type, llvm::Value *expr, int line, llvm::Value *size = nullptr);
    InternalTools::pair vectorTypePromotion(llvm::Value *leftExpr, llvm::Value *rightExpr, int line);
    llvm::Value *createVecFromVec(llvm::Value *exprP, llvm::Type *type, llvm::Value *maxSize, int line);
    llvm::Value *createVecFromScalar(llvm::Value *expr, llvm::Type *type, llvm::Value *size, int line);

    int getType(llvm::Type *expr);

private:
    std::string typeTable[7][7] = { {"bool", "char", "int", "real", "null", "vector", "matrix"},
                                    {"bool", "char", "int", "real", "null", "vector", "matrix"},
                                    {"bool", "char", "int", "real", "null", "vector", "matrix"},
                                    {"null", "null", "int", "real", "null", "vector", "matrix"},
                                    {"null", "null", "null", "null", "interval", "vector", "null"},
                                    {"null", "null", "null", "null", "null", "vector", "null"},
                                    {"null", "null", "null", "null", "null", "null", "matrix"}};

    std::string typePTable[7][7] = { {"bool", "null", "null", "null", "null", "vector", "matrix"},
                                     {"null", "char", "null", "null", "null", "vector", "matrix"},
                                     {"null", "null", "int", "real", "null", "vector", "matrix"},
                                     {"null", "null", "real", "real", "null", "vector", "matrix"},
                                     {"null", "null", "null", "null", "interval", "vector", "null"},
                                     {"vector", "vector", "vector", "vector", "vector", "vector", "null"},
                                     {"matrix", "matrix", "matrix", "matrix", "null", "null", "matrix"}};

    std::string typeAssTable[7][7] = { {"bool", "null", "null", "null", "null", "null", "null"},
                                       {"null", "char", "null", "null", "null", "null", "null"},
                                       {"null", "null", "int", "null", "null", "null", "null"},
                                       {"null", "null", "real", "real", "null", "null", "null"},
                                       {"null", "null", "null", "null", "interval", "null", "null"},
                                       {"vector", "vector", "vector", "vector", "vector", "vector", "null"},
                                       {"matrix", "matrix", "matrix", "matrix", "null", "null", "matrix"}};



protected:
    llvm::LLVMContext * globalCtx;
    llvm::IRBuilder<> * ir;
    InternalTools * it;
    ExternalTools * et;
    llvm::Module * mod;
    ErrorBuilder * eb;
};

#endif //GAZPREABASE_CASTTABLE_H
