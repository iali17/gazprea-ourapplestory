//
// Created by kyle on 02/12/18.
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

/**
 *
 * @param opNode
 * @param leftVec
 * @param rightVec
 * @return
 */
llvm::Value *CodeGenerator::getMatixRowOp(ASTNode *opNode, llvm::Value *leftVec, llvm::Value *rightVec) {
    return performInfixVectorOp(opNode, leftVec, rightVec);
}

llvm::Value *CodeGenerator::getUnaryMatrixRowOpVal(ASTNode *opNode, llvm::Value *curVec) {
    return performUnaryVectorOp(opNode, curVec);
}

/**
 * Deals with matrix arithmetic operations
 *
 * @param opNode
 * @param left
 * @param right
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::performInfixMatrixOp(ASTNode *opNode, llvm::Value *left, llvm::Value *right) {
//variables for the return
    llvm::Value * retNumRows = it->getValFromStruct(left, MATRIX_NUMROW_INDEX);
    llvm::Value * retNumCols = it->getValFromStruct(left, MATRIX_NUMCOL_INDEX);
    llvm::Value * ret        = et->getNewMatrix(it->getConsi32(INTEGER));
    ret = ir->CreatePointerCast(ret, matrixTy->getPointerTo());
    et->initMatrix(ret, retNumRows, retNumCols);
    llvm::Value * retVecsPtr = it->getPtrFromStruct(ret, MATRIX_ELEM_INDEX);
    llvm::Value * retVecPtr  = nullptr;
    llvm::Value * retTypePtr = it->getPtrFromStruct(ret, MATRIX_TYPE_INDEX);
    llvm::Type  * retType    = nullptr;

    //variables for the current loop iteration
    llvm::Value *curIdx         = nullptr;
    llvm::Value *curIdxPtr      = nullptr;
    llvm::Value *curLeftVec     = nullptr;
    llvm::Value *curRightVec    = nullptr;
    llvm::Value *curVec         = nullptr;
    llvm::Value *curVecPtr      = nullptr;
    llvm::Value *curLeftVecPtr  = nullptr;
    llvm::Value *curRightVecPtr = nullptr;
    llvm::Value *leftVecsPtr    = it->getPtrFromStruct(left,  MATRIX_ELEM_INDEX);
    llvm::Value *rightVecsPtr   = it->getPtrFromStruct(right, MATRIX_ELEM_INDEX);
    llvm::Value *cond           = nullptr;

    //loop var
    llvm::Value *curLoopVar = nullptr;

    //init loop index
    curIdxPtr = ir->CreateAlloca(intTy);
    curIdx    = it->getConsi32(0);
    ir->CreateStore(curIdx, curIdxPtr);

    auto * wb = new WhileBuilder(globalCtx, ir, mod);
    wb->beginWhile("MatrixInfixOp");

    //load the index
    curIdx = ir->CreateLoad(curIdxPtr);

    //loop control
    wb->beginInsertControl();
    cond = ir->CreateICmpSLT(curIdx, retNumRows);
    wb->insertControl(cond);

    //load the index
    curIdx = ir->CreateLoad(curIdxPtr);

    //get the current vectors
    curLeftVecPtr  = ir->CreateGEP(leftVecsPtr, curIdx);
    curRightVecPtr = ir->CreateGEP(rightVecsPtr, curIdx);

    //get the generated row
    curVecPtr = getMatixRowOp(opNode, curLeftVecPtr, curRightVecPtr);

    //save the type
    retType = it->getVectorElementType(curVecPtr);

    curVecPtr = ir->CreatePointerCast(curVecPtr, vecTy->getPointerTo());

    //get the current vector pointer
    retVecPtr = ir->CreateGEP(retVecsPtr, curIdx);

    //store the generated vector
    curVec = ir->CreateLoad(curVecPtr);
    ir->CreateStore(curVec, retVecPtr);

    //increment loop index
    curIdx = ir->CreateAdd(curIdx, it->getConsi32(1));
    ir->CreateStore(curIdx, curIdxPtr);

    wb->endWhile("EndMatrixInfixOp");

    //Type handling
    ret = it->castMatrixToType(ret, retType);
    llvm::Value * consType = it->getConstFromType(retType);
    ir->CreateStore(consType, retTypePtr);

    return ret;
}

/**
 * Handles unary matrix operations
 *
 * @param opNode
 * @param mat
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::performUnaryMatrixOp(ASTNode *opNode, llvm::Value *mat) {
//variables for the return
    llvm::Value * retNumRows = it->getValFromStruct(mat, MATRIX_NUMROW_INDEX);
    llvm::Value * retNumCols = it->getValFromStruct(mat, MATRIX_NUMCOL_INDEX);
    llvm::Value * ret        = et->getNewMatrix(it->getConsi32(INTEGER));
    ret = ir->CreatePointerCast(ret, matrixTy->getPointerTo());
    et->initMatrix(ret, retNumRows, retNumCols);
    llvm::Value * retVecsPtr = it->getPtrFromStruct(ret, MATRIX_ELEM_INDEX);
    llvm::Value * retVecPtr  = nullptr;
    llvm::Value * retTypePtr = it->getPtrFromStruct(ret, MATRIX_TYPE_INDEX);
    llvm::Type  * retType    = nullptr;


    //variables for the current loop iteration
    llvm::Value *curIdx       = nullptr;
    llvm::Value *curIdxPtr    = nullptr;
    llvm::Value *curMaVec     = nullptr;
    llvm::Value *curVec       = nullptr;
    llvm::Value *curVecPtr    = nullptr;
    llvm::Value *curMatVecPtr = nullptr;
    llvm::Value *matVecsPtr   = it->getPtrFromStruct(mat, MATRIX_ELEM_INDEX);
    llvm::Value *cond         = nullptr;

    //loop var
    llvm::Value *curLoopVar = nullptr;

    //init loop index
    curIdxPtr = ir->CreateAlloca(intTy);
    curIdx    = it->getConsi32(0);
    ir->CreateStore(curIdx, curIdxPtr);

    auto * wb = new WhileBuilder(globalCtx, ir, mod);
    wb->beginWhile("MatrixUnaryOp");

    //load the index
    curIdx = ir->CreateLoad(curIdxPtr);

    //loop control
    wb->beginInsertControl();
    cond = ir->CreateICmpSLT(curIdx, retNumRows);
    wb->insertControl(cond);

    //load the index
    curIdx = ir->CreateLoad(curIdxPtr);

    //get the current vectors
    curMatVecPtr  = ir->CreateGEP(matVecsPtr, curIdx);

    //get the generated row
    curVecPtr = getUnaryMatrixRowOpVal(opNode, curMatVecPtr);

    //save the type
    retType = curVecPtr->getType();

    curVecPtr = ir->CreatePointerCast(curVecPtr, vecTy->getPointerTo());

    //get the current vector pointer
    retVecPtr = ir->CreateGEP(retVecsPtr, curIdx);

    //store the generated vector
    curVec = ir->CreateLoad(curVecPtr);
    ir->CreateStore(curVec, retVecPtr);

    //increment loop index
    curIdx = ir->CreateAdd(curIdx, it->getConsi32(1));
    ir->CreateStore(curIdx, curIdxPtr);

    wb->endWhile("EndMatrixUnaryOp");

    //Type handling
    ret = it->castMatrixToType(ret, retType);
    llvm::Value * consType = it->getConstFromType(retType);
    ir->CreateStore(consType, retTypePtr);

    return ret;
}

/**
 * Handles matrix comparison operations
 *
 * @param opNode
 * @param left
 * @param right
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::performCompMatrixOp(ASTNode *opNode, llvm::Value *left, llvm::Value *right) {
    //variables for the return
    llvm::Value *ret     = it->geti1(1);
    llvm::Value *retPtr  = ir->CreateAlloca(boolTy);
    llvm::Value *numRows = it->getValFromStruct(left, MATRIX_NUMROW_INDEX);

    //variables for the current loop iteration
    llvm::Value *curIdx         = nullptr;
    llvm::Value *curIdxPtr      = nullptr;
    llvm::Value *curVal         = nullptr;
    llvm::Value *curLeftVecPtr  = nullptr;
    llvm::Value *curRightVecPtr = nullptr;
    llvm::Value *leftVecsPtr    = it->getPtrFromStruct(left,  MATRIX_ELEM_INDEX);
    llvm::Value *rightVecsPtr   = it->getPtrFromStruct(right, MATRIX_ELEM_INDEX);
    llvm::Value *cond           = nullptr;

    //loop var
    llvm::Value *curLoopVar = nullptr;

    //init loop index
    curIdxPtr = ir->CreateAlloca(intTy);
    curIdx    = it->getConsi32(0);
    ir->CreateStore(curIdx, curIdxPtr);

    auto * wb = new WhileBuilder(globalCtx, ir, mod);
    wb->beginWhile("MatrixCompare");

    //load the index
    curIdx = ir->CreateLoad(curIdxPtr);

    //loop control
    wb->beginInsertControl();
    cond = ir->CreateICmpSLT(curIdx, numRows);
    wb->insertControl(cond);

    //load the index
    curIdx = ir->CreateLoad(curIdxPtr);

    //get the current vectors
    curLeftVecPtr  = ir->CreateGEP(leftVecsPtr, curIdx);
    curRightVecPtr = ir->CreateGEP(rightVecsPtr, curIdx);

    //compare the current vectors
    curVal = performCompVectorOp(new EQNode(nullptr, nullptr, 0), curLeftVecPtr, curRightVecPtr);

    //and the result with the result of the proceeding comparisons
    ret = ir->CreateLoad(retPtr);
    ret = it->getAnd(ret, curVal);
    ir->CreateStore(ret, retPtr);

    //increment loop index
    curIdx = ir->CreateAdd(curIdx, it->getConsi32(1));
    ir->CreateStore(curIdx, curIdxPtr);

    wb->endWhile("EndMatrixCompare");

    ret = ir->CreateLoad(retPtr);

    //return
    if(dynamic_cast<EQNode *>(opNode))
        return ret;
    else if(dynamic_cast<NEQNode *>(opNode))
        return it->getNegation(ret);

    return ret;
}
