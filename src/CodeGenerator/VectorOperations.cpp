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
 * dot products of vectors
 * @param node
 * @return
 */
llvm::Value *CodeGenerator::visit(DotProductNode *node) {
    InternalTools::pair retVal = castAndPreserveSize(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    //check for non base type cases
    if(it->isVectorType(left)){
        return et->getDotProduct(left, right);
    }
    else {
        return performMatrixMultiplication(left, right);
    }
}

/**
 * performs the matrix multiplication, left and right casted already
 * @param left
 * @param right
 * @return
 */
llvm::Value *CodeGenerator::performMatrixMultiplication(llvm::Value *left, llvm::Value *right) {
    //return pointer type is handled by the et call
    llvm::Value * ret = et->getMatrixMultiplication(left, right);
    return ret;
}

/**
 * performs unary op
 * @param opNode
 * @param curVal
 * @return
 */
llvm::Value *CodeGenerator::getUnaryOpVal(ASTNode *opNode, llvm::Value *curVal) {
    if(dynamic_cast<NegateNode *>(opNode)){
        return it->getNegation(curVal);
    }
    return nullptr;
}

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
    else if(dynamic_cast<GTNode* >(opNode)) {
        return it->getGT(leftElmt, rightElmt);
    }
    else if(dynamic_cast<LTNode* >(opNode)) {
        return it->getLT(leftElmt, rightElmt);
    }
    else if(dynamic_cast<GTENode* >(opNode)) {
        return it->getGTE(leftElmt, rightElmt);
    }
    else if(dynamic_cast<LTENode* >(opNode)) {
        return it->getLTE(leftElmt, rightElmt);
    }
    else if(dynamic_cast<AndNode* >(opNode)) {
        return it->getAnd(leftElmt, rightElmt);
    }
    else if(dynamic_cast<OrNode* >(opNode)) {
        return it->getOr(leftElmt, rightElmt);
    }
    else if(dynamic_cast<XOrNode* >(opNode)) {
        return it->getXOr(leftElmt, rightElmt);
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
llvm::Value *CodeGenerator::performInfixVectorOp(ASTNode *opNode, llvm::Value *left, llvm::Value *right) {
    //variables for the return
    llvm::Value *retVec      = et->getOpResultVector(left, right);
    llvm::Value *retElmtsPtr = nullptr;
    llvm::Value *retElmtPtr  = nullptr;
    llvm::Value *retLenPtr   = nullptr;
    llvm::Value *retLen      = nullptr;

    //cover boolean return case
    if(dynamic_cast<CompNode *>(opNode) || dynamic_cast<BitArithNode *>(opNode)){
        llvm::Value *retTypePtr = it->getPtrFromStruct(retVec, it->getConsi32(VEC_TYPE_INDEX));
        ir->CreateStore(it->getConsi32(BOOLEAN), retTypePtr);
        retVec = it->castVectorToType(retVec, boolTy);
    }

    //finish ret init
    retElmtsPtr     = it->getPtrFromStruct(retVec, it->getConsi32(VEC_ELEM_INDEX));
    retElmtPtr      = nullptr;
    retLenPtr       = it->getPtrFromStruct(retVec, it->getConsi32(VEC_LEN_INDEX));
    retLen          = ir->CreateLoad(retLenPtr);

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

/**
 * perform == and != operations for vectors
 * @param opNode
 * @param left
 * @param right
 * @return
 */
llvm::Value *CodeGenerator::performCompVectorOp(ASTNode *opNode, llvm::Value *left, llvm::Value *right) {
    //variables for the return
    llvm::Value *ret = it->geti1(1);
    llvm::Value *retPtr = ir->CreateAlloca(boolTy);
    ir->CreateStore(ret, retPtr);
    retPtr->setName("CMPRetPtr");

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
    wb->beginWhile("BeginVectorComp");

    curIdx = ir->CreateLoad(curIdxPtr);

    wb->beginInsertControl();
    wb->insertControl(ir->CreateICmpSLT(curIdx, loopIters), "ControlVectorComp");

    //get left
    leftElmtPtr = ir->CreateGEP(leftElmtsPtr, curIdx);
    leftElmt    = ir->CreateLoad(leftElmtPtr);

    //get right
    rightElmtPtr = ir->CreateGEP(rightElmtsPtr, curIdx);
    rightElmt    = ir->CreateLoad(rightElmtPtr);

    //do eq comparision
    curVal = it->getEQ(leftElmt, rightElmt);

    //and the result with the current return
    ret = ir->CreateLoad(retPtr);
    ret = it->getAnd(ret, curVal);
    ir->CreateStore(ret, retPtr);

    //increment loop var
    curIdx = it->getAdd(curIdx, it->getConsi32(1));
    ir->CreateStore(curIdx, curIdxPtr);

    wb->endWhile("EndVectorComp");

    ret = ir->CreateLoad(retPtr);

    //return
    if(dynamic_cast<EQNode *>(opNode))
        return ret;
    else if(dynamic_cast<NEQNode *>(opNode))
        return it->getNegation(ret);
    else
        printf("invalid use of performCompVectorOp\n");
    return nullptr;
}

/**
 * basically only
 * @param opNode
 * @param left
 * @param right
 * @return
 */
llvm::Value *CodeGenerator::performUnaryVectorOp(ASTNode *opNode, llvm::Value *vec) {
    //variables for the return
    llvm::Value *retVec      = et->getVectorCopy(vec);
    llvm::Value *retElmtsPtr = it->getPtrFromStruct(retVec, it->getConsi32(VEC_ELEM_INDEX));
    llvm::Value *retElmtPtr  = nullptr;
    llvm::Value *retLenPtr   = it->getPtrFromStruct(retVec, it->getConsi32(VEC_LEN_INDEX));
    llvm::Value *retLen      = ir->CreateLoad(retLenPtr);

    //variables for the current loop iteration
    llvm::Value *curIdx    = it->getConsi32(0);
    llvm::Value *curIdxPtr = ir->CreateAlloca(intTy);
    llvm::Value *curVal    = nullptr;

    //init loop vars
    ir->CreateStore(curIdx, curIdxPtr);
    llvm::Value *retElmt = nullptr;

    //loop
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    wb->beginWhile();

    curIdx = ir->CreateLoad(curIdxPtr);

    wb->beginInsertControl();
    wb->insertControl(ir->CreateICmpSLT(curIdx, retLen));

    //get cur
    retElmtPtr = ir->CreateGEP(retElmtsPtr, curIdx);
    retElmt    = ir->CreateLoad(retElmtPtr);

    //do unary op
    curVal = getUnaryOpVal(opNode, retElmt);

    //re-store
    ir->CreateStore(curVal, retElmtPtr);

    //increment loop var
    curIdx = it->getAdd(curIdx, it->getConsi32(1));
    ir->CreateStore(curIdx, curIdxPtr);

    wb->endWhile();

    return retVec;
}
