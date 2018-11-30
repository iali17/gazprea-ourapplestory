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
    llvm::Value * interval = visit(node->getLeft());
    llvm::Value * iterator = visit(node->getRight());

    if(it->isVectorType(interval)){
        return et->getVectorBy(interval, iterator);
    }

    return ir->CreatePointerCast(et->getVectorFromInterval(interval, iterator), intVecTy->getPointerTo());
//    // uncomment this to print the resulting vector
//    auto result = ir->CreatePointerCast(et->getVectorFromInterval(interval, iterator), intVecTy->getPointerTo());
//    et->printVector(result);
//    return result;
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
    } else {
        intervalPtr = visit((IntervalNode *) node->getExpr());
    }
    symbolTable->addSymbol(id, node->getType(), node->isConstant(), intervalPtr);
    return nullptr;
}


llvm::Value *CodeGenerator::IntervalArith(ASTNode * node, llvm::Value *left, llvm::Value *right) {

    if ((it->isIntervalType(left) && it->isMatrixType(right)) ||
        (it->isMatrixType(left) && it->isIntervalType(right)) ){
        std::cerr << "Cannot implicitly convert interval to matrix on line: " << node->getLine() << ". Aborting...";
        exit(1);
    }

    // if left or right is vecType, then cast the interval to a vec then it's a vec arith
    // if left or right is intType, then cast the integer into interval
    llvm::Value * newLeft = left;
    llvm::Value * newRight = right;
    if (left->getType() == intTy){
        newLeft = et->getNewInterval(left,left);
        assert(it->isIntervalType(right));
    } else if (right->getType() == intTy) {
        newRight = et->getNewInterval(right, right);
        assert(it->isIntervalType(left));
    } else if (it->isVectorType(left)){
        newLeft = et->getVectorBy(left, it->getConsi32(1));
        assert(it->isIntervalType(right));
        // go to vec arith
    } else if (it->isVectorType(right)){
        newRight = et->getVectorBy(right, it->getConsi32(1));
        assert(it->isIntervalType(left));
    }

    // [a, b] +-*/ [c, d]
    llvm::Value * a = it->getValFromStruct(newLeft, INTERVAL_MIN);
    llvm::Value * b = it->getValFromStruct(newLeft, INTERVAL_MAX);
    llvm::Value * c = it->getValFromStruct(newRight, INTERVAL_MIN);
    llvm::Value * d = it->getValFromStruct(newRight, INTERVAL_MAX);
//    llvm::Value * resultLeft;
//    llvm::Value * resultRight;

    if(dynamic_cast<AddNode *>(node)) {     // [e,f] = [a + c, b + d]
        return et->getNewInterval(it->getAdd(a, c), it->getAdd(b, d));
    }

    else if(dynamic_cast<SubNode *>(node)) {     // [e,f] = [a - d, b - c]
        return et->getNewInterval(it->getSub(a, d), it->getSub(b, c));
    }

    else if(dynamic_cast<MulNode *>(node)) {

    }

    else if(dynamic_cast<DivNode *>(node)) {

    }

    return nullptr;
}



