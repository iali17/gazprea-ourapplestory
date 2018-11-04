//
// Created by kyle on 20/10/18.
//

#include <InternalTools/CondBuilder.h>
#include <iostream>

#include "InternalTools/CondBuilder.h"

llvm::Value *CondBuilder::beginIf(llvm::Value *cond, std::string label) {
    assert(status == CREATED);

    curFunction = ir->GetInsertBlock()->getParent();

    basicBlocks = new std::vector<llvm::BasicBlock *>();

    llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(*globalCtx, label, curFunction);
    llvm::BasicBlock *nextBB = llvm::BasicBlock::Create(*globalCtx, "Next");

    basicBlocks->push_back(thenBB);
    basicBlocks->push_back(nextBB);

    ir->CreateCondBr(cond, thenBB, nextBB);
    ir->SetInsertPoint(thenBB);

    status = STARTED;
    return nullptr;
}

llvm::Value *CondBuilder::beginElseIf(llvm::Value *cond, std::string label) {
    assert(status == STARTED);

    llvm::BasicBlock *thenBB = llvm::BasicBlock::Create(*globalCtx, label);
    llvm::BasicBlock *nextBB = llvm::BasicBlock::Create(*globalCtx, "Next");
    basicBlocks->push_back(thenBB);
    basicBlocks->push_back(nextBB);

    ir->CreateCondBr(cond, thenBB, nextBB);
    curFunction->getBasicBlockList().push_back(thenBB);
    ir->SetInsertPoint(thenBB);
    return nullptr;
}

llvm::Value *CondBuilder::beginElse(std::string label) {
    assert(status == STARTED);
    hasElse = true;

    status = MUSTFINALIZE;
    return nullptr;
}

llvm::Value *CondBuilder::finalize(std::string label) {
    llvm::BasicBlock *curBB = endLast();

    curFunction->getBasicBlockList().push_back(mergeBB);
    ir->SetInsertPoint(mergeBB);
    status = FINALIZED;
    return nullptr;
}

llvm::BasicBlock *CondBuilder::endLast(std::string label) {

    assert(basicBlocks->size() > 1);
    unsigned long bbLen = basicBlocks->size();
    llvm::BasicBlock *lastBB = basicBlocks->at(bbLen - 2);
    llvm::BasicBlock *curBB  = basicBlocks->at(bbLen - 1);

    curBB->setName("Alt_" + lastBB->getName());

    //conclude last if block
    ir->CreateBr(mergeBB);
    return curBB;
}

CondBuilder::CondBuilder(llvm::LLVMContext *globalCtx, llvm::IRBuilder<> *ir, llvm::Module *mod) : globalCtx(globalCtx),
                                                                                                       ir(ir), mod(mod) {
    mergeBB = llvm::BasicBlock::Create(*globalCtx, "merge");
    hasElse = false;
    status  = CREATED;
}

llvm::Value *CondBuilder::endIf() {
    assert(status == STARTED);
    llvm::BasicBlock *curBB = endLast();
    curFunction->getBasicBlockList().push_back(curBB);
    ir->SetInsertPoint(curBB);
    return nullptr;
}
