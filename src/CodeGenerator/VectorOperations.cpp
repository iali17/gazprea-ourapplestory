//
// Created by kyle on 24/11/18.
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
extern llvm::Type *intVecTy;
extern llvm::Type *charVecTy;
extern llvm::Type *boolVecTy;
extern llvm::Type *realVecTy;

/**
 * Return the proper operation based on the nodes class
 * @param opNode
 * @param leftElmt
 * @param rightElmt
 * @return
 */
llvm::Value *CodeGenerator::getArithOpVal(ASTNode *opNode, llvm::Value *leftElmt, llvm::Value *rightElmt) {
    if(dynamic_cast<AddNode *>(opNode)){
        return it->getAdd(leftElmt, rightElmt);
    }
    else if (dynamic_cast<SubNode *>(opNode)) {
        return it->getSub(leftElmt, rightElmt);
    }
    else if (dynamic_cast<MulNode *>(opNode)) {
        return it->getMul(leftElmt, rightElmt);
    }
    else if (dynamic_cast<DivNode *>(opNode)) {
        return it->getDiv(leftElmt, rightElmt);
    }
    else if (dynamic_cast<RemNode *>(opNode)) {
        return it->getRem(leftElmt, rightElmt);
    }
    else if (dynamic_cast<ExpNode *>(opNode)) {
        return et->aliPow(leftElmt, rightElmt);
    }
    return nullptr;
}

/**
 * Perform a vector operation for only arithmetic operations
 * @param opNode - node used to determine op type
 * @param left
 * @param right
 * @return - result vector
 */
llvm::Value *CodeGenerator::performArithVectorOp(ASTNode *opNode, llvm::Value *left, llvm::Value *right) {
    //variables for the return
    llvm::Value *retVec          = et->getOpResultVector(left, right);
    llvm::Value *retElmtsPtr     = it->getPtrFromStruct(retVec, it->getConsi32(VEC_ELEM_INDEX));
    llvm::Value *retElmtPtr      = nullptr;
    llvm::Value *retLenPtr       = it->getPtrFromStruct(retVec, it->getConsi32(VEC_LEN_INDEX));
    llvm::Value *retLen          = ir->CreateLoad(retLenPtr);

    //variables for left
    llvm::Value *leftElmtsPtr = it->getPtrFromStruct(left, it->getConsi32(VEC_ELEM_INDEX));
    llvm::Value *leftElmtPtr  = nullptr;

    //variables for right
    llvm::Value *rightElmtsPtr = it->getPtrFromStruct(right, it->getConsi32(VEC_ELEM_INDEX));
    llvm::Value *rightElmtPtr  = nullptr;

    //variables for the current loop iteration
    llvm::Value *curIdx    = it->getConsi32(0);
    llvm::Value *curIdxPtr = ir->CreateAlloca(intTy);
    llvm::Value *curVal    = nullptr;

    //init loop vars
    ir->CreateStore(curIdx, curIdxPtr);
    llvm::Value *leftElmt  = nullptr;
    llvm::Value *rightElmt = nullptr;

    //loop
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    wb->beginWhile();

    curIdx = ir->CreateLoad(curIdxPtr);

    wb->beginInsertControl();
    wb->insertControl(ir->CreateICmpSLT(curIdx, retLen));

    //get left
    leftElmtPtr = ir->CreateGEP(leftElmtsPtr, curIdx);
    leftElmt    = ir->CreateLoad(leftElmtPtr);

    //get right
    rightElmtPtr = ir->CreateGEP(rightElmtsPtr, curIdx);
    rightElmt    = ir->CreateLoad(rightElmtPtr);

    //get pointer to ret location
    retElmtPtr = ir->CreateGEP(retElmtsPtr, curIdx);

    //get op
    curVal = getArithOpVal(opNode, leftElmt, rightElmt);

    //store result
    ir->CreateStore(curVal, retElmtPtr);

    //increment loop var
    curIdx = it->getAdd(curIdx, it->getConsi32(1));
    ir->CreateStore(curIdx, curIdxPtr);

    wb->endWhile();

    //return
    return retVec;
}

llvm::Value *CodeGenerator::performCompVectorOp(ASTNode *opNode, llvm::Value *left, llvm::Value *right) {
    //variables for the return
    llvm::Value *ret = it->getConsi32(1);

    //variables for left
    llvm::Value *leftElmtsPtr = it->getPtrFromStruct(left, it->getConsi32(VEC_ELEM_INDEX));
    llvm::Value *leftElmtPtr  = nullptr;

    //variables for right
    llvm::Value *rightElmtsPtr = it->getPtrFromStruct(right, it->getConsi32(VEC_ELEM_INDEX));
    llvm::Value *rightElmtPtr  = nullptr;

    //variables for the current loop iteration
    llvm::Value *curIdx    = it->getConsi32(0);
    llvm::Value *curIdxPtr = ir->CreateAlloca(intTy);
    llvm::Value *curVal    = nullptr;

    //init loop vars
    ir->CreateStore(curIdx, curIdxPtr);
    llvm::Value *leftElmt  = nullptr;
    llvm::Value *rightElmt = nullptr;
    llvm::Value *loopIters = ir->CreateLoad(it->getPtrFromStruct(right, it->getConsi32(VEC_LEN_INDEX)));

    //loop
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    wb->beginWhile();

    curIdx = ir->CreateLoad(curIdxPtr);

    wb->beginInsertControl();
    wb->insertControl(ir->CreateICmpSLT(curIdx, loopIters));

    //get left
    leftElmtPtr = ir->CreateGEP(leftElmtsPtr, curIdx);
    leftElmt    = ir->CreateLoad(leftElmtPtr);

    //get right
    rightElmtPtr = ir->CreateGEP(rightElmtsPtr, curIdx);
    rightElmt    = ir->CreateLoad(rightElmtPtr);

    //get op
    curVal = getArithOpVal(opNode, leftElmt, rightElmt);

    //increment loop var
    curIdx = it->getAdd(curIdx, it->getConsi32(1));
    ir->CreateStore(curIdx, curIdxPtr);

    wb->endWhile();

    //return
    return ret;
}

