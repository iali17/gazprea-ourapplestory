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


llvm::Value *CodeGenerator::visit(IntervalNode *node) {
    llvm::Value * left = visit(node->getLeftBound());
    llvm::Value * right = visit(node->getRightBound());

    return et->getNewInterval(left, right);
}


/**
 * Creates vector from interval
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(ByNode *node) {
    llvm::Value * interval = visit(node->getLeft());
    llvm::Value * iterator = visit(node->getRight());

    if(it->isVectorType(interval)){
        return et->getVectorBy(interval, iterator);
    }

    return ir->CreatePointerCast(et->getVectorFromInterval(interval, iterator), intVecTy->getPointerTo());
}

/**
 * Handles interval declarations
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(IntervalDeclNode *node) {
    llvm::Value * intervalPtr;
    const std::string &id = node->getID();

    // if null or identity
    if (dynamic_cast<NullNode *>(node->getExpr())){
        intervalPtr = et->getNewInterval(it->getConsi32(0), it->getConsi32(0));
    }
    else if (dynamic_cast<IdnNode *>(node->getExpr())) {
        intervalPtr = et->getNewInterval(it->getConsi32(1), it->getConsi32(1));
    }
    else {
        intervalPtr = visit(node->getExpr());
    }
    symbolTable->addSymbol(id, node->getType(), node->isConstant(), intervalPtr);
    return nullptr;
}

/**
 * Handles interval arithmetic
 *
 * @param node
 * @param left
 * @param right
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::IntervalArith(ASTNode * node, llvm::Value *left, llvm::Value *right) {

    assert(it->isIntervalType(left) && it->isIntervalType(right));
    // [a, b] +-*/ [c, d]
    llvm::Value * a = it->getValFromStruct(left, INTERVAL_MIN);
    llvm::Value * b = it->getValFromStruct(left, INTERVAL_MAX);
    llvm::Value * c = it->getValFromStruct(right, INTERVAL_MIN);
    llvm::Value * d = it->getValFromStruct(right, INTERVAL_MAX);

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
                ir->CreateICmpSGT(a, it->getConsi32(0)),
                ir->CreateICmpSGT(c, it->getConsi32(0))));
        ir->CreateStore(ir->CreateMul(a, c), resultLeft);
        ir->CreateStore(ir->CreateMul(b, d), resultRight);
        cb->endIf();

        // a>0, d<0 [bc,ad]
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpSGT(a, it->getConsi32(0)),
                ir->CreateICmpSLT(d, it->getConsi32(0))));
        ir->CreateStore(ir->CreateMul(b, c), resultLeft);
        ir->CreateStore(ir->CreateMul(a, d), resultRight);
        cb->endIf();

        // b<0, c>0 [ad,bc]
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpSLT(b, it->getConsi32(0)),
                ir->CreateICmpSGT(c, it->getConsi32(0))));
        ir->CreateStore(ir->CreateMul(a, d), resultLeft);
        ir->CreateStore(ir->CreateMul(b, c), resultRight);
        cb->endIf();

        // b<0, d<0 [bd,ac]
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpSLT(b, it->getConsi32(0)),
                ir->CreateICmpSLT(d, it->getConsi32(0))));
        ir->CreateStore(ir->CreateMul(b, d), resultLeft);
        ir->CreateStore(ir->CreateMul(a, c), resultRight);
        cb->endIf();

        // a<0, b>0, c>0 [ad,bd]
        cb->beginElseIf(it->getAnd(it->getAnd(
                ir->CreateICmpSLT(a, it->getConsi32(0)),
                ir->CreateICmpSGT(b, it->getConsi32(0))),
                ir->CreateICmpSGT(c, it->getConsi32(0))));
        ir->CreateStore(ir->CreateMul(a, d), resultLeft);
        ir->CreateStore(ir->CreateMul(b, d), resultRight);
        cb->endIf();

        // a<0, b>0, d<0 [bc,ac]
        cb->beginElseIf(it->getAnd(it->getAnd(
                ir->CreateICmpSLT(a, it->getConsi32(0)),
                ir->CreateICmpSGT(b, it->getConsi32(0))),
                ir->CreateICmpSLT(d, it->getConsi32(0))));
        ir->CreateStore(ir->CreateMul(b, c), resultLeft);
        ir->CreateStore(ir->CreateMul(a, c), resultRight);
        cb->endIf();

        // a>0, c<0, d>0 [bc,bd]
        cb->beginElseIf(it->getAnd(it->getAnd(
                ir->CreateICmpSGT(a, it->getConsi32(0)),
                ir->CreateICmpSLT(c, it->getConsi32(0))),
                ir->CreateICmpSGT(d, it->getConsi32(0))));
        ir->CreateStore(ir->CreateMul(b, c), resultLeft);
        ir->CreateStore(ir->CreateMul(b, d), resultRight);
        cb->endIf();

        // b<0, c<0, d>0 [ad,ac]
        cb->beginElseIf(it->getAnd(it->getAnd(
                ir->CreateICmpSLT(b, it->getConsi32(0)),
                ir->CreateICmpSLT(c, it->getConsi32(0))),
                ir->CreateICmpSGT(d, it->getConsi32(0))));
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
                ir->CreateICmpSGT(a, it->getConsi32(0)),
                ir->CreateICmpSGT(c, it->getConsi32(0))));
        ir->CreateStore(it->getDiv(a, d), resultLeft);
        ir->CreateStore(it->getDiv(b, c), resultRight);
        cb->endIf();

        // b<0, c>0, [a/c,b/d]
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpSLT(b, it->getConsi32(0)),
                ir->CreateICmpSGT(c, it->getConsi32(0))));
        ir->CreateStore(it->getDiv(a, c), resultLeft);
        ir->CreateStore(it->getDiv(b, d), resultRight);
        cb->endIf();

        // a>0, d<0, [b/d,a/c]
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpSGT(a, it->getConsi32(0)),
                ir->CreateICmpSLT(d, it->getConsi32(0))));
        ir->CreateStore(it->getDiv(b, d), resultLeft);
        ir->CreateStore(it->getDiv(a, c), resultRight);
        cb->endIf();

        // b<0, d<0, [b/c,a/d]
        cb->beginElseIf(it->getAnd(
                ir->CreateICmpSLT(b, it->getConsi32(0)),
                ir->CreateICmpSLT(d, it->getConsi32(0))));
        ir->CreateStore(it->getDiv(b, d), resultLeft);
        ir->CreateStore(it->getDiv(a, c), resultRight);
        cb->endIf();

        // a<0, b>0, c>0 [a/c,b/c]
        cb->beginElseIf(it->getAnd(it->getAnd(
                ir->CreateICmpSLT(a, it->getConsi32(0)),
                ir->CreateICmpSGT(b, it->getConsi32(0))),
                        ir->CreateICmpSGT(c, it->getConsi32(0))));
        ir->CreateStore(it->getDiv(a, c), resultLeft);
        ir->CreateStore(it->getDiv(b, c), resultRight);
        cb->endIf();

        // a<0, b>0, d<0 [b/d,a/d]
        cb->beginElseIf(it->getAnd(it->getAnd(
                ir->CreateICmpSLT(a, it->getConsi32(0)),
                ir->CreateICmpSGT(b, it->getConsi32(0))),
                        ir->CreateICmpSLT(d, it->getConsi32(0))));
        ir->CreateStore(it->getDiv(b, c), resultLeft);
        ir->CreateStore(it->getDiv(a, c), resultRight);
        cb->endIf();

        // c<0, d>0 [-inf, +inf]
        // divisor contains zero so technically this is invalid
        cb->beginElse();
        ir->CreateStore(it->getConsi32(0), resultLeft);
        ir->CreateStore(it->getConsi32(0), resultRight);
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
        return it->getAnd(ir->CreateICmpEQ(a, c), ir->CreateICmpEQ(b, d));
    }
    else if(dynamic_cast<NEQNode *>(node)) {
        return it->getAnd(ir->CreateICmpNE(a, c), ir->CreateICmpNE(b, d));
    }
    std::cerr << "invalid arithmetic operation on line: " << node->getLine() << ". Aborting...";
    exit(1);
}

/**
 * Handles interval unary operations
 *
 * @param node
 * @param right
 * @return
 */
llvm::Value *CodeGenerator::IntervalUnary(ASTNode * node, llvm::Value *right) {
    llvm::Value * a = it->getValFromStruct(right, INTERVAL_MIN);
    llvm::Value * b = it->getValFromStruct(right, INTERVAL_MAX);

    if (dynamic_cast<NegateNode *>(node)) {
        llvm::Value * resultLeft = ir->CreateAlloca(intTy);
        llvm::Value * resultRight = ir->CreateAlloca(intTy);

        ir->CreateStore(it->getMul(it->getConsi32(-1), a), resultLeft);
        ir->CreateStore(it->getMul(it->getConsi32(-1), b), resultRight);

        // remember to flip left and right if left > right
        auto * cb = new CondBuilder(globalCtx, ir, mod);
        cb->beginIf(ir->CreateICmpSGT(ir->CreateLoad(resultLeft), ir->CreateLoad(resultRight)));
        llvm::Value * temp = ir->CreateAlloca(intTy);
        ir->CreateStore(ir->CreateLoad(resultLeft), temp);
        ir->CreateStore(ir->CreateLoad(resultRight), resultLeft);
        ir->CreateStore(ir->CreateLoad(temp), resultRight);
        cb->endIf();
        cb->finalize();

        return et->getNewInterval(ir->CreateLoad(resultLeft), ir->CreateLoad(resultRight));
    }
    std::cerr << "invalid unary operation on line: " << node->getLine() << ". Aborting...";
    exit(1);
}

