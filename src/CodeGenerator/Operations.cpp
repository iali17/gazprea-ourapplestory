//
// Created by kyle on 29/10/18.
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
extern llvm::Type *strTy;

/**
 * performs basic visit and type promotion
 * @param node
 * @return
 */
InternalTools::pair CodeGenerator::castForOp(InfixNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());

    assert(retVal.left->getType() == retVal.right->getType());

    return retVal;
}

InternalTools::pair CodeGenerator::castAndPreserveColSizeMatrix(InfixNode *node, llvm::Value *left, llvm::Value *right) {
    //save size
    llvm::Value *leftCols     = nullptr;
    llvm::Value *rightCols    = nullptr;
    llvm::Value *leftRowsPtr  = nullptr;
    llvm::Value *rightRowsPtr = nullptr;

    //some bools
    bool leftIsScalar  = not(it->isStructType(left));
    bool rightIsScalar = not(it->isStructType(right));

    if(not(leftIsScalar))
        leftCols  = it->getValFromStruct(left,  it->getConsi32(MATRIX_NUMCOL_INDEX));

    if(not(rightIsScalar))
        rightCols = it->getValFromStruct(right, it->getConsi32(MATRIX_NUMCOL_INDEX));

    //promote
    InternalTools::pair retVal;
    retVal = ct->typePromotion(left, right, node->getLine(), 1);

    //give size back
    if(not(leftIsScalar)){
        leftRowsPtr  = it->getPtrFromStruct(retVal.left,  it->getConsi32(MATRIX_NUMROW_INDEX));
        et->resizeMatrix( retVal.left,  ir->CreateLoad(leftRowsPtr),  leftCols);
    }

    if(not(rightIsScalar)){
        rightRowsPtr = it->getPtrFromStruct(retVal.right, it->getConsi32(MATRIX_NUMROW_INDEX));
        et->resizeMatrix(retVal.right, ir->CreateLoad(rightRowsPtr), rightCols);
    }

    assert(retVal.left->getType() == retVal.right->getType());

    return retVal;
}

InternalTools::pair CodeGenerator::castAndPreserveSizeScalar(InfixNode *node, llvm::Value *left, llvm::Value *right) {
    //save size
    llvm::Value *leftSize     = nullptr;
    llvm::Value *rightSize    = nullptr;

    //some bools
    bool leftIsScalar  = not(it->isStructType(left));
    bool rightIsScalar = not(it->isStructType(right));

    if(not(leftIsScalar))
        leftSize  = it->getValFromStruct(left,  it->getConsi32(VEC_LEN_INDEX));

    if(not(rightIsScalar))
        rightSize = it->getValFromStruct(right, it->getConsi32(VEC_LEN_INDEX));

    //promote
    InternalTools::pair retVal;
    retVal = ct->typePromotion(left, right, node->getLine(), 1);

    //give size back
    if(not(leftIsScalar)){
        et->resizeVector(retVal.left, leftSize);
    } else {
        et->resizeVector(retVal.left, it->getConsi32(1));
    }

    if(not(rightIsScalar)){
        et->resizeVector(retVal.right, rightSize);
    } else {
        et->resizeVector(retVal.right, it->getConsi32(1));
    }

    return retVal;
}

InternalTools::pair CodeGenerator::castAndPreserveSizeMatrix(InfixNode *node, llvm::Value *left, llvm::Value *right) {
    //save size
    llvm::Value *leftRows     = nullptr;
    llvm::Value *rightRows    = nullptr;
    llvm::Value *leftCols     = nullptr;
    llvm::Value *rightCols    = nullptr;
    llvm::Value *leftRowsPtr  = nullptr;
    llvm::Value *rightRowsPtr = nullptr;
    llvm::Value *leftColsPtr  = nullptr;
    llvm::Value *rightColsPtr = nullptr;

    //some bools
    bool leftIsScalar  = not(it->isStructType(left));
    bool rightIsScalar = not(it->isStructType(right));

    if(not(leftIsScalar)){
        leftRows     = it->getValFromStruct(left,  it->getConsi32(MATRIX_NUMROW_INDEX));
        leftCols     = it->getValFromStruct(left,  it->getConsi32(MATRIX_NUMCOL_INDEX));
    }

    if(not(rightIsScalar)){
        rightRows    = it->getValFromStruct(right, it->getConsi32(MATRIX_NUMROW_INDEX));
        rightCols    = it->getValFromStruct(right, it->getConsi32(MATRIX_NUMCOL_INDEX));
    }

    //promote
    InternalTools::pair retVal;
    retVal = ct->typePromotion(left, right, node->getLine(), 1);

    //give size back

    if(not(leftIsScalar)){
        leftRowsPtr  = it->getPtrFromStruct(retVal.left,  it->getConsi32(MATRIX_NUMROW_INDEX));
        leftColsPtr  = it->getPtrFromStruct(retVal.left,  it->getConsi32(MATRIX_NUMCOL_INDEX));

        ir->CreateStore(leftRows, leftRowsPtr);
        ir->CreateStore(leftCols, leftColsPtr);

        et->resizeMatrix( retVal.left,  leftRows,  leftCols);
    }

    if(not(rightIsScalar)){
        rightRowsPtr = it->getPtrFromStruct(retVal.right, it->getConsi32(MATRIX_NUMROW_INDEX));
        rightColsPtr = it->getPtrFromStruct(retVal.right, it->getConsi32(MATRIX_NUMCOL_INDEX));

        ir->CreateStore(rightRows, rightRowsPtr);
        ir->CreateStore(rightCols, rightColsPtr);

        et->resizeMatrix(retVal.right, rightRows, rightCols);
    }

    assert(retVal.left->getType() == retVal.right->getType());

    return retVal;
}

/**
 * cast but keep same dim for vector concat
 * @param node
 * @param left
 * @param right
 * @return
 */
InternalTools::pair CodeGenerator::castAndPreserveSizeVector(InfixNode *node, llvm::Value *left, llvm::Value *right) {
    //save size
    llvm::Value *leftSize     = nullptr;
    llvm::Value *rightSize    = nullptr;
    llvm::Value *leftSizePtr  = nullptr;
    llvm::Value *rightSizePtr = nullptr;

    //some bools
    bool leftIsScalar  = not(it->isStructType(left));
    bool rightIsScalar = not(it->isStructType(right));

    bool toStr = (left->getType() == strTy->getPointerTo()) || (right->getType() == strTy->getPointerTo());

    if(not(leftIsScalar))
        leftSize  = it->getValFromStruct(left,  it->getConsi32(VEC_LEN_INDEX));

    if(not(rightIsScalar))
        rightSize = it->getValFromStruct(right, it->getConsi32(VEC_LEN_INDEX));

    //promote
    InternalTools::pair retVal;
    retVal = ct->typePromotion(left, right, node->getLine(), 1);

    //give size back
    if(not(leftIsScalar)){
        leftSizePtr  = it->getPtrFromStruct(retVal.left,  it->getConsi32(VEC_LEN_INDEX));
        ir->CreateStore(leftSize, leftSizePtr);
    }
    if(not(rightIsScalar)){
        rightSizePtr = it->getPtrFromStruct(retVal.right, it->getConsi32(VEC_LEN_INDEX));
        ir->CreateStore(rightSize, rightSizePtr);
    }

    if(toStr){
        retVal.left  = ir->CreatePointerCast(retVal.left,  strTy->getPointerTo());
        retVal.right = ir->CreatePointerCast(retVal.right, strTy->getPointerTo());
    }

    assert(retVal.left->getType() == retVal.right->getType());

    return retVal;
}

/**
 * performs basic visit and type promotion but keeps the same dim
 * @param node
 * @return
 */
InternalTools::pair CodeGenerator::castAndPreserveSize(InfixNode *node, bool colsOnly) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());

    //checks for concat between scalar and vector
    if((it->isVectorType(left) && !it->isStructType(right)) || (it->isVectorType(right) && !it->isStructType(left))) {
        return castAndPreserveSizeScalar(node, left, right);
    }
    //check for non base type cases
     else if(it->isVectorType(left) || it->isVectorType(right)){
        return castAndPreserveSizeVector(node, left, right);
    }
    else if(colsOnly && (it->isMatrixType(left) || it->isMatrixType(right))){
        return castAndPreserveColSizeMatrix(node, left, right);
    }
    else if(it->isMatrixType(left) || it->isMatrixType(right)){
        return castAndPreserveSizeMatrix(node, left, right);
    }
    return *(new InternalTools::pair);
}

llvm::Value *CodeGenerator::visit(AddNode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    if (it->isIntervalType(left) || it->isIntervalType(right)){
        return IntervalArith(node, left, right);
    }

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }

    assert((left->getType() == intTy) || (left->getType() == realTy));
    return it->getAdd(left, right);
 }

llvm::Value *CodeGenerator::visit(SubNode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    if (it->isIntervalType(left) || it->isIntervalType(right)){
        return IntervalArith(node, left, right);
    }

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }

    assert((left->getType() == intTy) || (left->getType() == realTy));
    return it->getSub(left, right);
}

llvm::Value *CodeGenerator::visit(MulNode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    if (it->isIntervalType(left) || it->isIntervalType(right)){
        return IntervalArith(node, left, right);
    }

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }

    assert((left->getType() == intTy) || (left->getType() == realTy));
    return it->getMul(left, right);
}

llvm::Value *CodeGenerator::visit(DivNode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    if (it->isIntervalType(left) || it->isIntervalType(right)){
        return IntervalArith(node, left, right);
    }

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }

    assert((left->getType() == intTy) || (left->getType() == realTy));
    return it->getDiv(left, right);
}

llvm::Value *CodeGenerator::visit(RemNode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }

    assert((left->getType() == intTy) || (left->getType() == realTy));
    return it->getRem(left, right);
}

//TODO - split
llvm::Value *CodeGenerator::visit(ExpNode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }

    assert((left->getType() == intTy) || (left->getType() == realTy));
    if((left->getType() == intTy) || (left->getType() == realTy)){
        return et->aliPow(left, right);
    }

    std::cerr << "Unrecognized type during arithmetic operation on line " << node->getLine() << ". Aborting...\n";
    return nullptr;
}

llvm::Value *CodeGenerator::visit(EQNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());

    if(it->isTupleType(left)){
        return performTupleOp(left, right, EQ, node->getLine());
    }

    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    left  = retVal.left;
    right = retVal.right;

    //check for non base type cases
    if (it->isIntervalType(left) || it->isIntervalType(right)){
        return IntervalArith(node, left, right);
    }

    if(it->isVectorType(left)){
        return performCompVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performCompMatrixOp(node, left, right);
    }

    return it->getEQ(left, right);
}

llvm::Value *CodeGenerator::visit(NEQNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());

    if (it->isTupleType(left)){
        return performTupleOp(left, right, NEQ, node->getLine());
    }

    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    left  = retVal.left;
    right = retVal.right;

    //check for non base type cases
    if (it->isIntervalType(left) || it->isIntervalType(right)){
        return IntervalArith(node, left, right);
    }

    if(it->isVectorType(left)){
        return performCompVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performCompMatrixOp(node, left, right);
    }

    return it->getNEQ(left, right);
}

llvm::Value *CodeGenerator::visit(GTNode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }
    return it->getGT(left, right);
}

llvm::Value *CodeGenerator::visit(LTNode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }
    return it->getLT(left, right);
}

llvm::Value *CodeGenerator::visit(GTENode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }
    return it->getGTE(left, right);
}

llvm::Value *CodeGenerator::visit(LTENode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }
    return it->getLTE(left, right);
}

llvm::Value *CodeGenerator::visit(AndNode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }
    return it->getAnd(left, right);
}

llvm::Value *CodeGenerator::visit(OrNode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }
    return it->getOr(left, right);
}

llvm::Value *CodeGenerator::visit(XOrNode *node) {
    InternalTools::pair retVal = castForOp(dynamic_cast<InfixNode *>(node));
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    //check for non base type cases
    if(it->isVectorType(left)){
        return performInfixVectorOp(node, left, right);
    }
    else if(it->isMatrixType(left)){
        return performInfixMatrixOp(node, left, right);
    }
    return it->getXOr(left, right);
}

llvm::Value *CodeGenerator::visit(NegateNode *node) {
    llvm::Value * expr  = visit(node->getExpr());
    //check for non base type cases

    if (it->isIntervalType(expr)) {
        return IntervalUnary(node, expr);
    }
    else if(it->isVectorType(expr)){
        return performUnaryVectorOp(node, expr);
    }
    else if(it->isMatrixType(expr)){
        return performUnaryMatrixOp(node, expr);
    }
    return it->getNegation(expr);
}

/**
 * for EQ or NEQ vector operations
 * @param left
 * @param right
 * @param OPTYPE
 * @return
 */
llvm::Value *CodeGenerator::performTupleOp(llvm::Value *left, llvm::Value * right, int OPTYPE, int line){
	llvm::Type *tmpType;
	llvm::StructType *leftType, *rightType;
	GazpreaTupleType *leftTupleType, *rightTupleType;
	llvm::Value *ret, *leftMember, *rightMember, *tmp;
	unsigned long numLeftMembers, numRightMembers;
	
	//get typ info for left operand
	tmpType = left->getType()->getPointerElementType();
	assert(tmpType->isStructTy());
	leftType = llvm::cast<llvm::StructType>(tmpType);
	leftTupleType = symbolTable->resolveTupleType(leftType);
	numLeftMembers = leftTupleType->getMembers()->size();

	//get type info for right operand
	tmpType = right->getType()->getPointerElementType();
	assert(tmpType->isStructTy());
	rightType = llvm::cast<llvm::StructType>(tmpType);
	rightTupleType = symbolTable->resolveTupleType(rightType);
	numRightMembers = rightTupleType->getMembers()->size();

	assert(numLeftMembers == numRightMembers);

	//default return true
	if(OPTYPE == EQ)
	    ret = it->geti1(1);
	else
	    ret = it->geti1(0);

	for(unsigned long i = 0; i < numLeftMembers; ++i){
        InternalTools::pair retVal;
	    //get cur member
	    leftMember  = it->getValFromTuple(left,  it->getConsi32(i));
        rightMember = it->getValFromTuple(right, it->getConsi32(i));

        retVal = ct->typePromotion(leftMember, rightMember, line);
        leftMember = retVal.left;
        rightMember = retVal.right;

        if (OPTYPE == EQ){
            tmp = it->getEQ(leftMember, rightMember);
            ret = ir->CreateAnd(ret, tmp);
        } else if (OPTYPE == NEQ){
            tmp = it->getNEQ(leftMember, rightMember);
            ret = ir->CreateOr(ret, tmp);
        }
        //update return value
	}
    return ret;
}

/**
 * Concatenates things
 * @param node
 * @return
 */
llvm::Value *CodeGenerator::visit(ConcatenationNode *node) {
    InternalTools::pair retVal = castAndPreserveSize(dynamic_cast<InfixNode *>(node), true);
    llvm::Value * left         = retVal.left;
    llvm::Value * right        = retVal.right;

    //check for non base type cases
    if(it->isVectorType(left)){
        return et->concatenateVectors(left, right);
    }
    else if(it->isMatrixType(left)){
        return et->concatenateMatrices(left, right);
    }
    return nullptr;
}
