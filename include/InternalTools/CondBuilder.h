//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_CONDBUILDER_H
#define GAZPREABASE_CONDBUILDER_H
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#define CREATED      0
#define STARTED      1
#define MUSTFINALIZE 2
/**
 * HOW TO USE:
 *  - Create builder
 *  - create if, write your block
 *  - create as many else ifs as you want with you blocks following them
 *  - you can create and else if you want, with you block of code following it
 *  - finalize you statement
 */
class CondBuilder {
public:
    llvm::Value *createIf(llvm::Value *cond, std::string label = "Then");
    llvm::Value *createElseIf(llvm::Value *cond, std::string label = "ElseIf");
    llvm::Value *createElse(std::string label = "Else");
    llvm::Value *finalize(std::string label = "End");

    CondBuilder(llvm::LLVMContext *globalCtx, llvm::IRBuilder<> *ir, llvm::Module *mod);

protected:
    llvm::LLVMContext * globalCtx;
    llvm::IRBuilder<> * ir;
    llvm::Module      * mod;

    bool hasElse;
    int  status;

    llvm::Function   * curFunction;
    llvm::BasicBlock * mergeBB;

    std::vector<llvm::BasicBlock *> * basicBlocks;

    //void createBlocks();
    llvm::BasicBlock *endLast(std::string label = "");
};
#endif //GAZPREABASE_CONDBUILDER_H
