//
// Created by kyle on 20/10/18.
//

#include <InternalTools/InternalTools.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;

InternalTools::pair InternalTools::makePair(llvm::Value *leftV, llvm::Value *rightV) {
    pair pair1;
    pair1.left = leftV;
    pair1.right = rightV;
    return pair1;
}

/**
 *	returns the actual value
 */
llvm::Value *InternalTools::getValFromTuple(llvm::Value *tuplePtr, llvm::Value *idx) {
	llvm::Value *ptr = ir->CreateInBoundsGEP(tuplePtr, {getConsi32(0), idx});
	llvm::Value *val = ir->CreateLoad(ptr);
	return ir->CreateLoad(val);
}


/**
 *	returns a pointer to the the tuple element. YOu can store to here
 */
llvm::Value *InternalTools::getPtrFromTuple(llvm::Value *tuplePtr, llvm::Value *idx){
    llvm::Value *ptr = ir->CreateInBoundsGEP(tuplePtr, {getConsi32(0), idx});
    return ir->CreateLoad(ptr);
}

llvm::Value *InternalTools::getReal(float val) {
    return llvm::ConstantFP::get(realTy, val);
}

llvm::Value *InternalTools::geti1(int64_t val) {
    uint64_t trueValue = static_cast<uint64_t>(static_cast<int64_t>(val));
    return llvm::ConstantInt::get(boolTy, trueValue, true);
}

llvm::Value *InternalTools::geti8(int64_t val) {
    uint64_t trueValue = static_cast<uint64_t>(static_cast<int64_t>(val));
    return llvm::ConstantInt::get(i8Ty, trueValue, true);
}

llvm::Value *InternalTools::getConsi32(int64_t val) {
    uint64_t trueValue = static_cast<uint64_t>(static_cast<int64_t>(val));
    return llvm::ConstantInt::get(i32Ty, trueValue, true);
}

llvm::Value *InternalTools::getConsi64(int64_t val) {
    uint64_t trueValue = static_cast<uint64_t>(static_cast<int64_t>(val));
    return llvm::ConstantInt::get(i64Ty, trueValue, true);
}

bool InternalTools::setNull(llvm::Type * type, llvm::Value * ptr) {
    if(type == boolTy){
        ir->CreateStore(geti1(0), ptr);
        return true;
    }
    else if (type == i8Ty){
        ir->CreateStore(geti8(0), ptr);
        return true;
    }
    else if(type == intTy){
        ir->CreateStore(getConsi32(0), ptr);
        return true;
    }
    else if(type == realTy){
        ir->CreateStore(getReal(0), ptr);
        return true;
    }
    return false;
}

InternalTools::InternalTools(llvm::LLVMContext *globalCtx, llvm::IRBuilder<> *ir, llvm::Module *mod) : globalCtx(
        globalCtx), ir(ir), mod(mod) {
    setUpTypes();
}

llvm::Value *InternalTools::safeReplace(llvm::Value *safePtr, llvm::Value *cond, llvm::Value *idx, llvm::Value *arrP,
                                        llvm::Value *replaceVal) {
    llvm::Function *curFunction = ir->GetInsertBlock()->getParent();
    llvm::BasicBlock *thenBB    = llvm::BasicBlock::Create(*globalCtx, "then", curFunction);
    llvm::BasicBlock *elseBB    = llvm::BasicBlock::Create(*globalCtx, "else");
    llvm::BasicBlock *mergeBB   = llvm::BasicBlock::Create(*globalCtx, "ifcont");

    //eval condition
    ir->CreateCondBr(cond, thenBB, elseBB);

    ir->SetInsertPoint(thenBB);
    llvm::Value *ptr = ir->CreateGEP(arrP, idx);
    llvm::Value *val = ir->CreateLoad(ptr, "val");
    ir->CreateStore(val, safePtr);
    ir->CreateBr(mergeBB);

    curFunction->getBasicBlockList().push_back(elseBB);
    ir->SetInsertPoint(elseBB);
    ir->CreateStore(replaceVal, safePtr);
    ir->CreateBr(mergeBB);

    curFunction->getBasicBlockList().push_back(mergeBB);
    ir->SetInsertPoint(mergeBB);
    return nullptr;
}

void InternalTools::setUpTypes() {
    i64Ty  = llvm::TypeBuilder<llvm::types::i<64>, true>::get(*globalCtx);
    i32Ty  = llvm::TypeBuilder<llvm::types::i<32>, true>::get(*globalCtx);
    intTy  = llvm::TypeBuilder<llvm::types::i<32>, true>::get(*globalCtx);
    i8Ty   = llvm::TypeBuilder<llvm::types::i<8>,  true>::get(*globalCtx);
    charTy = llvm::TypeBuilder<llvm::types::i<8>,  true>::get(*globalCtx);
    boolTy = llvm::TypeBuilder<llvm::types::i<1>,  true>::get(*globalCtx);
    realTy = llvm::Type::getFloatTy(*globalCtx);
}

llvm::Value *InternalTools::getNull(llvm::Type *type) {
    if(type == boolTy)
        return geti1(0);
    else if(type == charTy)
        return geti8(0);
    else if(type == intTy)
        return getConsi32(0);
    else if(type == realTy)
        return getReal(0.0);
    else
        return nullptr;
}

llvm::Value *InternalTools::getIdn(llvm::Type *type) {
    if(type == boolTy)
        return geti1(1);
    else if(type == charTy)
        return geti8(1);
    else if(type == intTy)
        return getConsi32(1);
    else if(type == realTy)
        return getReal(1.0);
    else
        return nullptr;
}

llvm::Value *InternalTools::initTuple(llvm::Value *tuplePtr, std::vector<llvm::Value *> *values) {
    //fill new structure
    auto *structType = llvm::cast<llvm::StructType>(tuplePtr->getType()->getPointerElementType());
    auto types   = structType->elements();
    for(unsigned long i = 0; i < values->size(); i++){
        llvm::Value *structElem = ir->CreateInBoundsGEP(tuplePtr, {getConsi32(0), getConsi32(i)});
        llvm::Value *ptr        = ir->CreateAlloca(types[i]->getPointerElementType());
        ir->CreateStore(values->at(i), ptr);
        ir->CreateStore(ptr, structElem);
    }
    return tuplePtr ;
}

llvm::Value *InternalTools::getAdd(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateAdd(left, right, "iaddtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFAdd(left, right, "faddtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";
    return nullptr;
}

llvm::Value *InternalTools::getSub(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateSub(left, right, "iaddtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFSub(left, right, "faddtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *InternalTools::getMul(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateMul(left, right, "imultmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFMul(left, right, "fmultmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *InternalTools::getDiv(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateSDiv(left, right, "idivtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFDiv(left, right, "fdivtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *InternalTools::getRem(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateSRem(left, right, "iremtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFRem(left, right, "fremtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *InternalTools::getEQ(llvm::Value *left, llvm::Value *right) {
    if (left->getType() == boolTy){
        return ir->CreateAnd(left, right, "fuck");
    }
    else if(left->getType() == intTy){
        return ir->CreateICmpEQ(left, right, "ieqtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpUEQ(left, right, "feqtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *InternalTools::getNEQ(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == boolTy){
        return getNegation(ir->CreateAnd(left, right, "igttmp"));
    }
    if(left->getType() == intTy){
        return ir->CreateICmpNE(left, right, "ineqtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpUNE(left, right, "fneqtmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *InternalTools::getGT(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateICmpSGT(left, right, "igttmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpUGT(left, right, "fgttmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *InternalTools::getLT(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateICmpSLT(left, right, "ilttmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpULT(left, right, "flttmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *InternalTools::getLTE(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateICmpSLE(left, right, "iltetmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpOLE(left, right, "fltetmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *InternalTools::getGTE(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateICmpSGE(left, right, "igtetmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpOGE(left, right, "fgtetmp");
    }

    std::cerr << "Unrecognized type during arithmetic operation\n";

    return nullptr;
}

llvm::Value *InternalTools::getAnd(llvm::Value *left, llvm::Value *right) {
    return ir->CreateAnd(left, right, "andtmp");
}

llvm::Value *InternalTools::getOr(llvm::Value *left, llvm::Value *right) {
    return ir->CreateOr(left, right, "andtmp");
}

llvm::Value *InternalTools::getXOr(llvm::Value *left, llvm::Value *right) {
    return ir->CreateXor(left, right, "andtmp");
}

llvm::Value *InternalTools::getNegation(llvm::Value *expr) {
    if (expr->getType() == realTy){
        return ir->CreateFNeg(expr, "fnegtmp");
    }

    return ir->CreateNeg(expr, "negtmp");
}

bool InternalTools::isStructType(llvm::Value *ptr) {
    if(ptr->getType()->isPointerTy()){
        return ptr->getType()->getPointerElementType()->isStructTy();
    }
    return false;
}
