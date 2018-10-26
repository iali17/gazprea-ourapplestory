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

llvm::Value *InternalTools::getReal(double val) {
    return llvm::ConstantFP::get(realTy, val);
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
    realTy = llvm::Type::getFloatTy(*globalCtx);
    boolTy = llvm::Type::getInt1Ty(*globalCtx);
}