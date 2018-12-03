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
    // uncomment below and comment above to print the resulting vector
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

    if(dynamic_cast<AddNode *>(node)) {     // [e,f] = [a + c, b + d]
        return et->getNewInterval(it->getAdd(a, c), it->getAdd(b, d));
    }

    else if(dynamic_cast<SubNode *>(node)) {     // [e,f] = [a - d, b - c]
        return et->getNewInterval(it->getSub(a, d), it->getSub(b, c));
    }

    else if(dynamic_cast<MulNode *>(node)) {
        auto *cb = new CondBuilder(globalCtx, ir, mod);
        llvm::Value * resultLeft = ir->CreateAlloca(intTy);
        llvm::Value * resultRight = ir->CreateAlloca(intTy);

        // a>0, c>0, [ac,bd]
        cb->beginIf(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(a, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(c, it->getConsi32(0)))));
        ir->CreateStore(ir->CreateMul(a, c), resultLeft);
        ir->CreateStore(ir->CreateMul(b, d), resultRight);
        cb->endIf();

        // a>0, d<0 [bc,ad]
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(a, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(d, it->getConsi32(0)))));
        ir->CreateStore(ir->CreateMul(b, c), resultLeft);
        ir->CreateStore(ir->CreateMul(a, d), resultRight);
        cb->endIf();

        // b<0, c>0 [ad,bc]
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(b, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(c, it->getConsi32(0)))));
        ir->CreateStore(ir->CreateMul(a, d), resultLeft);
        ir->CreateStore(ir->CreateMul(b, c), resultRight);
        cb->endIf();

        // b<0, d<0 [bd,ac]
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(b, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(d, it->getConsi32(0)))));
        ir->CreateStore(ir->CreateMul(b, d), resultLeft);
        ir->CreateStore(ir->CreateMul(a, c), resultRight);
        cb->endIf();

        // a<0, b>0, c>0 [ad,bd]
        cb->beginElseIf(it->getAnd(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(a, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(b, it->getConsi32(0)))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(c, it->getConsi32(0)))));
        ir->CreateStore(ir->CreateMul(a, d), resultLeft);
        ir->CreateStore(ir->CreateMul(b, d), resultRight);
        cb->endIf();

        // a<0, b>0, d<0 [bc,ac]
        cb->beginElseIf(it->getAnd(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(a, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(b, it->getConsi32(0)))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(d, it->getConsi32(0)))));
        ir->CreateStore(ir->CreateMul(b, c), resultLeft);
        ir->CreateStore(ir->CreateMul(a, c), resultRight);
        cb->endIf();

        // a>0, c<0, d>0 [bc,bd]
        cb->beginElseIf(it->getAnd(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(a, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(c, it->getConsi32(0)))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(d, it->getConsi32(0)))));
        ir->CreateStore(ir->CreateMul(b, c), resultLeft);
        ir->CreateStore(ir->CreateMul(b, d), resultRight);
        cb->endIf();

        // b<0, c<0, d>0 [ad,ac]
        cb->beginElseIf(it->getAnd(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(b, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(c, it->getConsi32(0)))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(d, it->getConsi32(0)))));
        ir->CreateStore(ir->CreateMul(a, d), resultLeft);
        ir->CreateStore(ir->CreateMul(a, c), resultRight);
        cb->endIf();

        // min max stuff
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpSLT(ir->CreateMul(a, d), ir->CreateMul(b, c)),
                ir->CreateICmpSGT(ir->CreateMul(a, c), ir->CreateMul(b, d))
                ));
        ir->CreateStore(ir->CreateMul(a, d), resultLeft);
        ir->CreateStore(ir->CreateMul(a, c), resultRight);
        cb->endIf();

        cb->beginElseIf(it->getAnd(
                ir->CreateICmpSLT(ir->CreateMul(a, d), ir->CreateMul(b, c)),
                ir->CreateICmpSLT(ir->CreateMul(a, c), ir->CreateMul(b, d))
                ));
        ir->CreateStore(ir->CreateMul(a, d), resultLeft);
        ir->CreateStore(ir->CreateMul(b, d), resultRight);
        cb->endIf();

        cb->beginElseIf(it->getAnd(
                ir->CreateICmpSGT(ir->CreateMul(a, d), ir->CreateMul(b, c)),
                ir->CreateICmpSGT(ir->CreateMul(a, c), ir->CreateMul(b, d))
                ));
        ir->CreateStore(ir->CreateMul(b, c), resultLeft);
        ir->CreateStore(ir->CreateMul(a, c), resultRight);
        cb->endIf();

        cb->beginElse();
        ir->CreateStore(ir->CreateMul(b, c), resultLeft);
        ir->CreateStore(ir->CreateMul(b, d), resultRight);
        cb->finalize();

        // remember to flip left and right if left > right
        cb = new CondBuilder(globalCtx, ir, mod);
        cb->beginIf(ir->CreateICmpSGT(ir->CreateLoad(resultLeft), ir->CreateLoad(resultRight)));
            llvm::Value * temp = ir->CreateAlloca(intTy);
            ir->CreateStore(ir->CreateLoad(resultLeft), temp);
            ir->CreateStore(ir->CreateLoad(resultRight), resultLeft);
            ir->CreateStore(ir->CreateLoad(temp), resultRight);
        cb->endIf();
        cb->finalize();

        return et->getNewInterval(ir->CreateLoad(resultLeft), ir->CreateLoad(resultRight));
    }

    else if(dynamic_cast<DivNode *>(node)) {
        auto *cb = new CondBuilder(globalCtx, ir, mod);
        llvm::Value * resultLeft = ir->CreateAlloca(intTy);
        llvm::Value * resultRight = ir->CreateAlloca(intTy);

        // a>0, c>0, [a/d,b/c]
        cb->beginIf(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(a, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(c, it->getConsi32(0)))));
        ir->CreateStore(it->getDiv(a, d), resultLeft);
        ir->CreateStore(it->getDiv(b, c), resultRight);
        cb->endIf();

        // b<0, c>0, [a/c,b/d]
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(b, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(c, it->getConsi32(0)))));
        ir->CreateStore(it->getDiv(a, c), resultLeft);
        ir->CreateStore(it->getDiv(b, d), resultRight);
        cb->endIf();

        // a>0, d<0, [b/d,a/c]
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(a, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(d, it->getConsi32(0)))));
        ir->CreateStore(it->getDiv(b, d), resultLeft);
        ir->CreateStore(it->getDiv(a, c), resultRight);
        cb->endIf();

        // b<0, d<0, [b/c,a/d]
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(b, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(d, it->getConsi32(0)))));
        ir->CreateStore(it->getDiv(b, c), resultLeft);
        ir->CreateStore(it->getDiv(a, d), resultRight);
        cb->endIf();

        // a<0, b>0, c>0 [a/c,b/c]
        cb->beginElseIf(it->getAnd(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(a, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(b, it->getConsi32(0)))),
                                   ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(c, it->getConsi32(0)))));
        ir->CreateStore(it->getDiv(a, c), resultLeft);
        ir->CreateStore(it->getDiv(b, c), resultRight);
        cb->endIf();

        // a<0, b>0, d<0 [b/d,a/d]
        cb->beginElseIf(it->getAnd(it->getAnd(
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(a, it->getConsi32(0))),
                ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSGT(b, it->getConsi32(0)))),
                                   ir->CreateICmpEQ(it->geti1(1), ir->CreateICmpSLT(d, it->getConsi32(0)))));
        ir->CreateStore(it->getDiv(b, d), resultLeft);
        ir->CreateStore(it->getDiv(a, d), resultRight);
        cb->endIf();

        // c<0, d>0 [-inf, +inf]
        cb->beginElse();
//        ir->CreateStore(it->getNInf(), resultLeft);
//        ir->CreateStore(it->getInf(), resultRight);
        ir->CreateStore(it->getConsi32(2147483647), resultLeft);
        ir->CreateStore(it->getConsi32(-2147483648), resultRight);
        cb->finalize();

        // remember to flip left and right if left > right
        cb = new CondBuilder(globalCtx, ir, mod);
        cb->beginIf(ir->CreateICmpSGT(ir->CreateLoad(resultLeft), ir->CreateLoad(resultRight)));
            llvm::Value * temp = ir->CreateAlloca(intTy);
            ir->CreateStore(ir->CreateLoad(resultLeft), temp);
            ir->CreateStore(ir->CreateLoad(resultRight), resultLeft);
            ir->CreateStore(ir->CreateLoad(temp), resultRight);
        cb->endIf();
        cb->finalize();

        return et->getNewInterval(ir->CreateLoad(resultLeft), ir->CreateLoad(resultRight));
    }

    else if(dynamic_cast<EQNode *>(node)) {
        auto *cb = new CondBuilder(globalCtx, ir, mod);
        llvm::Value * resultLeft = ir->CreateAlloca(intTy);
        llvm::Value * resultRight = ir->CreateAlloca(intTy);

        cb->beginIf(it->getAnd(ir->CreateICmpEQ(a, c), ir->CreateICmpEQ(b, d)));
        ir->CreateStore(it->getConsi32(1), resultLeft);
        ir->CreateStore(it->getConsi32(1), resultRight);
        cb->endIf();
        cb->beginElse();
        ir->CreateStore(it->getConsi32(0), resultLeft);
        ir->CreateStore(it->getConsi32(0), resultRight);
        cb->finalize();

        return et->getNewInterval(ir->CreateLoad(resultLeft), ir->CreateLoad(resultRight));
    }
    else if(dynamic_cast<NEQNode *>(node)) {
        auto *cb = new CondBuilder(globalCtx, ir, mod);
        llvm::Value * resultLeft = ir->CreateAlloca(intTy);
        llvm::Value * resultRight = ir->CreateAlloca(intTy);

        cb->beginIf(it->getAnd(ir->CreateICmpNE(a, c), ir->CreateICmpNE(b, d)));
        ir->CreateStore(it->getConsi32(1), resultLeft);
        ir->CreateStore(it->getConsi32(1), resultRight);
        cb->endIf();
        cb->beginElse();
        ir->CreateStore(it->getConsi32(0), resultLeft);
        ir->CreateStore(it->getConsi32(0), resultRight);
        cb->finalize();

        return et->getNewInterval(ir->CreateLoad(resultLeft), ir->CreateLoad(resultRight));
    }
    std::cerr << "invalid arithmetic operation on line: " << node->getLine() << " Aborting...";
    exit(1);
}

llvm::Value *CodeGenerator::IntervalUnary(ASTNode * node, llvm::Value *right) {
    llvm::Value * a = it->getValFromStruct(right, INTERVAL_MIN);
    llvm::Value * b = it->getValFromStruct(right, INTERVAL_MAX);

    if (dynamic_cast<NegateNode *>(node)) {
        return et->getNewInterval(it->getMul(it->getConsi32(-1), a), it->getMul(it->getConsi32(-1), b));
    }
    std::cerr << "invalid unary operation on line: " << node->getLine() << " Aborting...";
    exit(1);
}

