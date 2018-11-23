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
    llvm::Value * interval = visit((IntervalNode *) node->getLeft());
    llvm::Value * expr = visit(node->getRight());
    auto result = et->getVectorFromInterval(interval, expr);
    return result;
}


llvm::Value *CodeGenerator::visit(IntervalDeclNode *node) {
    llvm::Value * intervalPtr;
    const std::string &id = node->getID();

    // if null or identity
    if(!(dynamic_cast<IntervalNode *>(node->getExpr()))) {
        if (dynamic_cast<NullNode *>(node->getExpr())){
            intervalPtr = et->getNewInterval(it->getConsi32(0), it->getConsi32(0));
        }
        else if (dynamic_cast<IdnNode *>(node->getExpr())) {
            intervalPtr = et->getNewInterval(it->getConsi32(1), it->getConsi32(1));
        }
    }
    else {
        intervalPtr = visit((IntervalNode *) node->getExpr());
    }
    symbolTable->addSymbol(id, INTEGER, node->isConstant(), intervalPtr);
    return nullptr;
}



