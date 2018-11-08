//
// Created by kyle on 22/10/18.
//

#ifndef GAZPREABASE_WHILEBUILDER_H
#define GAZPREABASE_WHILEBUILDER_H
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>

#define CREATED      0
#define STARTED      1
#define MUSTFINALIZE 2
#define FINALIZED    3
#define SKIP         5

/**
 * HOW TO USE:
 *
 * WhileBuilder *wb = new WhileBuilder(globalCtx, ir, mod);
 *
 * wb->beginWhile();
 *      < YOUR LLVM CODE HERE >
 *      MAKE SURE YOU LOAD THE VALUES YOU NEED TO LOAD BEFORE THE LOOP CONTROL
 * wb->beginInsertControl();
 * wb->insertControl(LLVM_CONDITION);
 *      < YOUR LLVM CODE HERE >
 *      MAKE SURE YOU STORE THE VALUES YOU NEED TO
 * wb->endWhile;
 *
 */
class WhileBuilder {
public:
    WhileBuilder(llvm::LLVMContext *globalCtx, llvm::IRBuilder<> *ir, llvm::Module *mod);
    llvm::Value *beginWhile(std::string label = "BeginWhile");
    llvm::Value *beginInsertControl(std::string bodyLabel = "LoopBody");
    llvm::Value *insertControl(llvm::Value *cond, std::string bodyLabel = "LoopBody");
    llvm::Value *endWhile(std::string label = "EndWhile");

    llvm::BasicBlock *getStartWhileBB() const;
    llvm::BasicBlock *getMergeBB() const;

protected:
    llvm::LLVMContext * globalCtx;
    llvm::IRBuilder<> * ir;
    llvm::Module      * mod;

    int  status;

    llvm::Function   * curFunction;
    llvm::BasicBlock * startWhileBB;
    llvm::BasicBlock * postControlBlock;
    llvm::BasicBlock * mergeBB;
};
#endif //GAZPREABASE_WHILEBUILDER_H