//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_CODEGENERATOR_H
#define GAZPREABASE_CODEGENERATOR_H

#include <AST/ASTVisitor/ASTBaseVisitor.h>

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_os_ostream.h>
#include <iostream>
#include <fstream>

#include "InternalTools/InternalTools.h"
#include "InternalTools/CondBuilder.h"
#include "InternalTools/WhileBuilder.h"
#include "ExternalTools/ExternalTools.h"
#include "Scope/SymbolTable.h"

class CodeGenerator : public ASTBaseVisitor {
public:
    CodeGenerator(char *outFile);

    void generate(ASTNode * node) {
        prepare();
        visit(node);
        write();
    }

    llvm::Value* visit(FileNode       *node) override;
    llvm::Value* visit(BasicBlockNode *node) override;
    llvm::Value* visit(ASTNode        *node) override;
    llvm::Value* visit(ProcedureNode  *node) override;
    llvm::Value* visit(ParamNode      *node) override;
    llvm::Value* visit(ReturnNode     *node) override;
    llvm::Value* visit(INTNode        *node) override;
    llvm::Value* visit(RealNode       *node) override;
    llvm::Value* visit(CharNode       *node) override;
    llvm::Value* visit(BoolNode       *node) override;
    llvm::Value* visit(CondNode       *node) override;
    llvm::Value* visit(LoopNode       *node) override;
    llvm::Value* visit(DoLoopNode     *node) override;
    llvm::Value* visit(InLoopNode     *node) override;
protected:
    llvm::LLVMContext * globalCtx;
    llvm::IRBuilder<> * ir;
    llvm::Module      * mod;
    SymbolTable       * symbolTable;

    InternalTools *it;
    ExternalTools *et;

    char * outFile;

    /**
     * initialize all the llvm stuff
     */
    void prepare() {
        globalCtx   = new llvm::LLVMContext;
        ir          = new llvm::IRBuilder<>(*globalCtx);
        mod         = new llvm::Module("gazprea", *globalCtx);
        et          = new ExternalTools(globalCtx, ir, mod);
        it          = new InternalTools(globalCtx, ir, mod);
        symbolTable = new SymbolTable();
    }

    /**
     * write the code to file
     */
    void write() {
        std::ofstream out(outFile);
        llvm::raw_os_ostream ros(out);
        llvm::raw_os_ostream llErr(std::cerr);
        // verify the module and print the stuff into the out file
        llvm::verifyModule(*mod, &llErr);
        mod->print(ros, nullptr);
    }
};
#endif //GAZPREABASE_CODEGENERATOR_H
