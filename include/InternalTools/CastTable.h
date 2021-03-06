//
// Created by ali5 on 10/26/18.
//

#ifndef GAZPREABASE_CASTTABLE_H
#define GAZPREABASE_CASTTABLE_H

#include <iostream>

#include <llvm/IR/IRBuilder.h>
#include "globals.h"
#include "InternalTools.h"
#include "CondBuilder.h"
#include "ErrorBuilder.h"
#include "WhileBuilder.h"
#include "ExternalTools/ExternalTools.h"
#include "AST/ASTNodes/ErrorNodes/ScalarErrorNode.h"
#include "AST/ASTNodes/ErrorNodes/VectorErrorNode.h"
#include "AST/ASTNodes/TerminalNodes/VectorNode.h"


class CastTable {
public:
    CastTable(llvm::LLVMContext *globalctx, llvm::IRBuilder<> *ir, InternalTools *it, ExternalTools *et, llvm::Module *mod, ErrorBuilder *eb);

    InternalTools::pair typePromotion(llvm::Value *leftExpr, llvm::Value *rightExpr, int line, int opt = -1);
    llvm::Value *varCast(llvm::Type *type, llvm::Value *expr, int line);

    llvm::Value *typeAssCast(llvm::Type *type, llvm::Value *expr, int line, llvm::Value *size = nullptr, llvm::Value *extraSize = nullptr, int exprSize = -1);
    llvm::Value *vecAssCast(llvm::Type *type, llvm::Value *expr, int line, llvm::Value *size = nullptr, int exprSize = -1);
    llvm::Value *matAssCast(llvm::Type *type, llvm::Value *expr, int line, llvm::Value *leftsize = nullptr, llvm::Value *rightSize = nullptr);

    InternalTools::pair IntervalTypePromotion(llvm::Value *leftExpr, llvm::Value *rightExpr, int line);
    InternalTools::pair vectorTypePromotion(llvm::Value *leftExpr, llvm::Value *rightExpr, int line , int opt = -1);
    InternalTools::pair vectorToVectorPromotion(llvm::Value *leftExpr, llvm::Value *rightExpr, int line, int opt = -1);
    InternalTools::pair matrixTypePromotion(llvm::Value *leftExpr, llvm::Value *rightExpr, int line, int opt = -1);
    InternalTools::pair matrixToMatrixPromotion(llvm::Value *leftExpr, llvm::Value *rightExpr, int line, int opt = -1);

    llvm::Value *createVecFromVec(llvm::Value *exprP, llvm::Type *type, llvm::Value *maxSize, int line);
    llvm::Value *createMatFromMat(llvm::Value *exprP, llvm::Type *type, llvm::Value *leftSize, llvm::Value *rightSize, int line);
    llvm::Value *createVecFromScalar(llvm::Value *expr, llvm::Type *type, llvm::Value *size, int line);
    llvm::Value *createMatFromScalar(llvm::Value *expr, llvm::Type *type, llvm::Value *leftSize, llvm::Value *rightSize, int line);

    llvm::Value *createMatViaSizeGiven(llvm::Value *mat, llvm::Value *expr, llvm::Type *type, llvm::Value *rowSize, llvm::Value *colSize, int line);

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
