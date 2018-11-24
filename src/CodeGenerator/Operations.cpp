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

llvm::Value *CodeGenerator::visit(AddNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    // todo
    if (node->getLeft()->getType() == INTERVAL || node->getRight()->getType() == INTERVAL){
        return IntervalAdd(left, right);
    }

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    assert((left->getType() == intTy) || (left->getType() == realTy));
    return it->getAdd(left, right);
}

llvm::Value *CodeGenerator::visit(SubNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    assert((left->getType() == intTy) || (left->getType() == realTy));
    return it->getSub(left, right);
}

llvm::Value *CodeGenerator::visit(MulNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    assert((left->getType() == intTy) || (left->getType() == realTy));
    return it->getMul(left, right);
}

llvm::Value *CodeGenerator::visit(DivNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    assert((left->getType() == intTy) || (left->getType() == realTy));
    return it->getDiv(left, right);
}

llvm::Value *CodeGenerator::visit(RemNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    assert((left->getType() == intTy) || (left->getType() == realTy));
    return it->getRem(left, right);
}

//TODO - split
llvm::Value *CodeGenerator::visit(ExpNode *node) {
    llvm::Value * left = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
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

    if ((left->getType()->isPointerTy()) && (left->getType()->getPointerElementType()->isStructTy())){
        return performTupleOp(left, right, EQ, node->getLine());
    }

    InternalTools::pair retVal;
    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());

    return it->getEQ(left, right);
}

llvm::Value *CodeGenerator::visit(NEQNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());

    if ((left->getType()->isPointerTy()) && (left->getType()->getPointerElementType()->isStructTy())){
        return performTupleOp(left, right, NEQ, node->getLine());
    }

    InternalTools::pair retVal;
    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    return it->getNEQ(left, right);
}

llvm::Value *CodeGenerator::visit(GTNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    return it->getGT(left, right);
}

llvm::Value *CodeGenerator::visit(LTNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    return it->getLT(left, right);
}

llvm::Value *CodeGenerator::visit(GTENode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    return it->getGTE(left, right);
}

llvm::Value *CodeGenerator::visit(LTENode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    return it->getLTE(left, right);
}

llvm::Value *CodeGenerator::visit(AndNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    return it->getAnd(left, right);
}

llvm::Value *CodeGenerator::visit(OrNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    return it->getOr(left, right);
}

llvm::Value *CodeGenerator::visit(XOrNode *node) {
    llvm::Value * left  = visit(node->getLeft());
    llvm::Value * right = visit(node->getRight());
    InternalTools::pair retVal;

    retVal = ct->typePromotion(left, right, node->getLine());
    left = retVal.left;
    right = retVal.right;

    assert(left->getType() == right->getType());
    return it->getXOr(left, right);
}

llvm::Value *CodeGenerator::visit(NegateNode *node) {
    llvm::Value * expr  = visit(node->getExpr());

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
