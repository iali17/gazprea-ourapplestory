//
// Created by kyle on 20/10/18.
//

#include <CodeGenerator/CodeGenerator.h>


extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;

llvm::Value *CodeGenerator::visit(FileNode *node) {
    //THIS IS A TEST !!!!! HERE I AM MAKING A MAIN FUNCTION TO TEST WITH
    llvm::FunctionType *mainTy = llvm::TypeBuilder<int(), false>::get(*globalCtx);

    et->registerPrintf();
    auto *mainFunc = llvm::cast<llvm::Function>(mod->getOrInsertFunction("main", mainTy));

    // Create an entry block and set the inserter.
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(*globalCtx, "entry", mainFunc);
    ir->SetInsertPoint(entry);

    //THIS IS A TEST
    CondBuilder *condBuilder = new CondBuilder(globalCtx, ir, mod);
    llvm::Value *ptr = ir->CreateAlloca(intTy);
    llvm::Value *val = it->getConsi32(5);

    ir->CreateStore(val, ptr);
    llvm::Value *cond0 = ir->CreateICmpSGE(ir->CreateLoad(ptr), it->getConsi32(3));

    condBuilder->createIf(cond0);
    ir->CreateStore(it->getConsi32(2), ptr);
    et->printInt(it->getConsi32(5));
    condBuilder->createElseIf(cond0, "hoii");
    ir->CreateStore(it->getConsi32(3), ptr);
    condBuilder->createElseIf(cond0);
    et->printInt(it->getConsi32(4));
    ir->CreateStore(it->getConsi32(5), ptr);
    condBuilder->createElse();
    ir->CreateStore(it->getConsi32(9), ptr);
    condBuilder->finalize();

    WhileBuilder *whileBuilder = new WhileBuilder(globalCtx, ir, mod);

    ir->CreateStore(it->getConsi32(0), ptr);
    whileBuilder->beginWhile();
    val = ir->CreateLoad(ptr);
    llvm::Value *cond = ir->CreateICmpSLT(val, it->getConsi32(5));
    whileBuilder->insertControl(cond);
    et->printInt(val);
    val = ir->CreateAdd(val, it->getConsi32(1));
    ir->CreateStore(val, ptr);
    whileBuilder->endWhile();

    auto trueValue = static_cast<uint64_t>(static_cast<int64_t>(0));
    llvm::Value *ret = llvm::ConstantInt::get(intTy, trueValue, true);
    ir->CreateRet(ret);
    return nullptr;
}

llvm::Value *CodeGenerator::visit(BlockNode *node) {
    return nullptr;
}

llvm::Value *CodeGenerator::visit(ASTNode *node) {
    return ASTBaseVisitor::visit(node);
}

CodeGenerator::CodeGenerator(char *outFile) : outFile(outFile) {
}
