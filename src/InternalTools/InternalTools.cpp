//
// Created by kyle on 20/10/18.
//

#include <InternalTools/InternalTools.h>

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
    i64Ty  = llvm::TypeBuilder<llvm::types::i<64>, true>::get(*globalCtx);
    i32Ty  = llvm::TypeBuilder<llvm::types::i<32>, true>::get(*globalCtx);
    intTy  = llvm::TypeBuilder<llvm::types::i<32>, true>::get(*globalCtx);
    i8Ty   = llvm::TypeBuilder<llvm::types::i<8>,  true>::get(*globalCtx);
    charTy = llvm::TypeBuilder<llvm::types::i<8>,  true>::get(*globalCtx);
}

