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
#define FINALIZED    3
/**
 * HOW TO USE:
 *
 *  CondBuilder *cb = new CondBuilder(globalCtx, ir, mod); //CREATE RIGHT BEFORE USE
 *
 *  cb->beginIf(LLVM_CONDITION);
 *      < YOUR LLVM CODE HERE >
 *  cb->endIf();
 *
 *  // BEGIN ELSE IF (OPTIONAL)
 *  // USE AS MANY AS YOU WANT
 *  cb->beginElseIf(LLVM_CONDITION);
 *      < YOUR LLVM CODE HERE >
 *  cb->endIf();
 *  // END ELSE IF
 *
 *  //BEGIN ELSE (OPTIONAL)
 *  cb->beginElse(LLVM_CONDITION);
 *      < YOUR LLVM CODE HERE >
 *  //END ELSE (NOTE THAT WE DO NOT CALL endIf() after an else
 *
 *  cb->finalize();
 *
 */
class CondBuilder {
public:
    llvm::Value *beginIf(llvm::Value *cond, std::string label = "Then");
    llvm::Value *beginElseIf(llvm::Value *cond, std::string label = "ElseIf");
    llvm::Value *beginElse(std::string label = "Else");
    llvm::Value *finalize(std::string label = "End");
    llvm::Value *endIf();

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
    llvm::BasicBlock *last;
};
#endif //GAZPREABASE_CONDBUILDER_H
