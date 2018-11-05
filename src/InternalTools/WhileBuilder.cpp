//
// Created by kyle on 22/10/18.
//

#include <InternalTools/WhileBuilder.h>

#include "InternalTools/WhileBuilder.h"

/**
 * standard constructor
 * @param globalCtx
 * @param ir
 * @param mod
 */
WhileBuilder::WhileBuilder(llvm::LLVMContext *globalCtx, llvm::IRBuilder<> *ir, llvm::Module *mod) : globalCtx(
        globalCtx), ir(ir), mod(mod) {
    status = CREATED;
}

/**
 * Call to begin inserting code in the while. This will either be when you load your control variables, or
 * do the "do" portion of a do while
 *
 * @param label - optional label of the start of the loop
 * @return
 */
llvm::Value *WhileBuilder::beginWhile(std::string label) {
    assert(status == CREATED);
    status = STARTED;
    curFunction = ir->GetInsertBlock()->getParent();

    startWhileBB = llvm::BasicBlock::Create(*globalCtx, label, curFunction);
    mergeBB      = llvm::BasicBlock::Create(*globalCtx, "merge");

    ir->CreateBr(startWhileBB);
    ir->SetInsertPoint(startWhileBB);
    return nullptr;
}

/**
 * Call this where you want the loop control to go. This will either be after the "do" portion of a do while,
 * or before the body of your standard loop
 *
 * @param cond
 * @param bodyLabel - optional label for the loop body
 * @return
 */
llvm::Value *WhileBuilder::insertControl(llvm::Value *cond, std::string bodyLabel) {
    assert(status == STARTED);
    status = MUSTFINALIZE;

    postControlBlock = llvm::BasicBlock::Create(*globalCtx, bodyLabel);

    ir->CreateCondBr(cond, postControlBlock, mergeBB);

    curFunction->getBasicBlockList().push_back(postControlBlock);
    ir->SetInsertPoint(postControlBlock);
    return nullptr;
}

/**
 * Call to end inserting code in your while block. All code placed after the call to this function
 * will reside outside of the while block
 *
 * @param label - optional label for the merge block
 * @return
 */
llvm::Value *WhileBuilder::endWhile(std::string label) {
    assert(status == MUSTFINALIZE);
    status = FINALIZED;

    //if (not(postControlBlock->getTerminator()))
    if (not(curFunction->getBasicBlockList().back().getTerminator()))
        ir->CreateBr(startWhileBB);

    mergeBB->setName(label);
    curFunction->getBasicBlockList().push_back(mergeBB);
    ir->SetInsertPoint(mergeBB);
    return nullptr;
}

/**
 * @return  - pointer to start of while block
 */
llvm::BasicBlock *WhileBuilder::getStartWhileBB() const {
    return startWhileBB;
}

/**
 * @return  - pointer to end of while block
 */
llvm::BasicBlock *WhileBuilder::getMergeBB() const {
    return mergeBB;
}
