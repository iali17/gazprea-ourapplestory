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
 *	returns the qactual value
 */
llvm::Value *InternalTools::getValFromTuple(llvm::Value *tuplePtr, llvm::Value *idx){
	llvm::Value *ptr = ir->CreateInBoundsGEP(tuplePtr, {getConsi32(0), idx});
	return ir->CreateLoad(ptr);
}

/**
 *	returns a pointer to the the tuple element. YOu can store to here
 */
llvm::Value *InternalTools::getPtrFromTuple(llvm::Value *tuplePtr, llvm::Value *idx){
	return ir->CreateInBoundsGEP(tuplePtr, {getConsi32(0), idx});
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

