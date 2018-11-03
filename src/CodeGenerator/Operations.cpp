//
// Created by kyle on 29/10/18.
//

#include "CodeGenerator/CodeGenerator.h"
extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;

llvm::Value *CodeGenerator::visit(AddNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    if(left->getType() == intTy){
        return ir->CreateAdd(left, right, "iaddtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFAdd(left, right, "faddtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *CodeGenerator::visit(SubNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    if(left->getType() == intTy){
        return ir->CreateSub(left, right, "iaddtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFSub(left, right, "faddtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *CodeGenerator::visit(MulNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    if(left->getType() == intTy){
        return ir->CreateMul(left, right, "imultmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFMul(left, right, "fmultmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *CodeGenerator::visit(DivNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    if(left->getType() == intTy){
        return ir->CreateSDiv(left, right, "idivtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFDiv(left, right, "fdivtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *CodeGenerator::visit(RemNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    if(left->getType() == intTy){
        return ir->CreateSRem(left, right, "iremtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFRem(left, right, "fremtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

// Todo: Somehow insert power function for llvm
llvm::Value *CodeGenerator::visit(ExpNode *node) {
    llvm::Value * left = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    if(left->getType() == intTy){
        WhileBuilder *wb = new WhileBuilder(globalCtx, ir, mod);
        llvm::Value *zero = it->getConsi32(0);
        llvm::Value *i;

        llvm::Value *iPtr = ir->CreateAlloca(intTy);
        ir->CreateStore(right, iPtr);

        llvm::Value *leftPtr = ir->CreateAlloca(intTy);
        ir->CreateStore(left, leftPtr);

        wb->beginWhile();

        i = ir->CreateLoad(iPtr);

        wb->insertControl(ir->CreateICmpNE(zero, i));

        llvm::Value *leftLoad = ir->CreateLoad(leftPtr);


        wb->endWhile();

        return ir->CreateSRem(left, right, "iremtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFRem(left, right, "fremtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";
}

llvm::Value *CodeGenerator::visit(EQNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    if(left->getType() == intTy){
        return ir->CreateICmpEQ(left, right, "ieqtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpUEQ(left, right, "feqtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *CodeGenerator::visit(NEQNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    if(left->getType() == intTy){
        return ir->CreateICmpNE(left, right, "ineqtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpUNE(left, right, "fneqtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *CodeGenerator::visit(GTNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    if(left->getType() == intTy){
        return ir->CreateICmpSGT(left, right, "igttmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpUGT(left, right, "fgttmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *CodeGenerator::visit(LTNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    if(left->getType() == intTy){
        return ir->CreateICmpSLT(left, right, "ilttmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpULT(left, right, "flttmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *CodeGenerator::visit(GTENode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    if(left->getType() == intTy){
        return ir->CreateICmpSGE(left, right, "igtetmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpOGE(left, right, "fgtetmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *CodeGenerator::visit(LTENode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    if(left->getType() == intTy){
        return ir->CreateICmpSLE(left, right, "iltetmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpOLE(left, right, "fltetmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *CodeGenerator::visit(AndNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    return ir->CreateAnd(left, right, "andtmp");
}

llvm::Value *CodeGenerator::visit(OrNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    return ir->CreateOr(left, right, "andtmp");
}

llvm::Value *CodeGenerator::visit(XOrNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right);
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    return ir->CreateXor(left, right, "andtmp");
}

llvm::Value *CodeGenerator::visit(NegateNode *node) {
    llvm::Value * expr  = visit(node->getExpr());

    if (expr->getType() == realTy){
        return ir->CreateFNeg(expr, "fnegtmp");
    }

    return ir->CreateNeg(expr, "negtmp");
}
