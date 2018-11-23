//
// Created by csun on 11/22/18.
//

#include <CodeGenerator/CodeGenerator.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;
extern llvm::Type *vecTy;
extern llvm::Type *matrixTy;
extern llvm::Type *intVecTy;
extern llvm::Type *intMatrixTy;
extern llvm::Type *charVecTy;
extern llvm::Type *charMatrixTy;
extern llvm::Type *boolVecTy;
extern llvm::Type *boolMatrixTy;
extern llvm::Type *realVecTy;
extern llvm::Type *realMatrixTy;
extern llvm::Type *intervalTy;


// todo: check this and by
llvm::Value *CodeGenerator::visit(IntervalNode *node) {
    llvm::Value * left = visit(node->getLeftBound());
    llvm::Value * right = visit(node->getRightBound());
    return et->getNewInterval(left, right);
}

llvm::Value *CodeGenerator::visit(ByNode *node) {
    llvm::Value * interval = visit((IntervalNode *)node->getLeft());
    llvm::Value * expr = visit(node->getRight());

    return et->getVectorFromInterval(interval, expr);
}


llvm::Value *CodeGenerator::visit(IntervalDeclNode *node) {
    auto expr = node->getExpr();
    auto intervalPtr = visit((IntervalNode *) expr);
    const std::string &id = node->getID();

    symbolTable->addSymbol(id, INTEGER, node->isConstant(), intervalPtr);
    return nullptr;
}
