//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_CODEGENERATOR_H
#define GAZPREABASE_CODEGENERATOR_H

#include <AST/AST.h>
#include <AST/ASTVisitor/ASTBaseVisitor.h>

#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/raw_os_ostream.h>
#include <iostream>
#include <fstream>
#include <string>

#include "InternalTools/InternalTools.h"
#include "InternalTools/CondBuilder.h"
#include "InternalTools/WhileBuilder.h"
#include "InternalTools/CastTable.h"
#include "InternalTools/ErrorBuilder.h"
#include "ExternalTools/ExternalTools.h"
#include "Scope/SymbolTable.h"

class CodeGenerator : public ASTBaseVisitor {
public:
    explicit CodeGenerator(char *outFile);

    void generate(ASTNode * node) {
        prepare();
        visit(node);
        write();
    }

    llvm::Value* visit(FileNode           *node) override;
    llvm::Value* visit(BasicBlockNode     *node) override;
    llvm::Value* visit(ASTNode            *node) override;
    llvm::Value* visit(ProcedureNode      *node) override;
    llvm::Value* visit(ParamNode          *node) override;
    llvm::Value* visit(ReturnNode         *node) override;
    llvm::Value* visit(INTNode            *node) override;
    llvm::Value* visit(RealNode           *node) override;
    llvm::Value* visit(CharNode           *node) override;
    llvm::Value* visit(BoolNode           *node) override;
    llvm::Value* visit(NullNode           *node) override;
    llvm::Value* visit(CondNode           *node) override;
    llvm::Value* visit(LoopNode           *node) override;
    llvm::Value* visit(DoLoopNode         *node) override;
    llvm::Value* visit(InLoopNode         *node) override;
    llvm::Value* visit(DeclNode           *node) override;
    llvm::Value* visit(AssignNode         *node) override;
    llvm::Value* visit(CastExprNode       *node) override;
    llvm::Value* visit(IDNode             *node) override;
    llvm::Value* visit(IdnNode            *node) override;
    llvm::Value* visit(InputNode          *node) override;
    llvm::Value* visit(OutputNode         *node) override;
    llvm::Value* visit(StreamDeclNode     *node) override;
    llvm::Value* visit(TypeDefNode        *node) override;
    llvm::Value* visit(CallNode           *node) override;
    llvm::Value* visit(AddNode            *node) override;
    llvm::Value* visit(SubNode            *node) override;
    llvm::Value* visit(MulNode            *node) override;
    llvm::Value* visit(DivNode            *node) override;
    llvm::Value* visit(RemNode            *node) override;
    llvm::Value* visit(ExpNode            *node) override;
    llvm::Value* visit(EQNode             *node) override;
    llvm::Value* visit(NEQNode            *node) override;
    llvm::Value* visit(GTNode             *node) override;
    llvm::Value* visit(LTNode             *node) override;
    llvm::Value* visit(GTENode            *node) override;
    llvm::Value* visit(LTENode            *node) override;
    llvm::Value* visit(AndNode            *node) override;
    llvm::Value* visit(OrNode             *node) override;
    llvm::Value* visit(XOrNode            *node) override;
    llvm::Value* visit(NegateNode         *node) override;
    llvm::Value* visit(ContinueNode       *node) override;
    llvm::Value* visit(BreakNode          *node) override;
    llvm::Value* visit(ProcedureCallNode  *node) override;
    llvm::Value* visit(ProtoProcedureNode *node) override;
    llvm::Value* visit(TupleDeclNode      *node) override;
    llvm::Value* visit(TupleType          *node) override;
    llvm::Value* visit(GlobalDeclNode     *node) override;
    llvm::Value* visit(GlobalRefNode      *node) override;
    llvm::Value* visit(TupleNode          *node) override;
    llvm::Value* visit(PythonTupleAssNode *node) override;
    llvm::Value* visit(IndexTupleNode     *node) override;
    llvm::Value* visit(TupleMemberAssNode *node) override;
    llvm::Value* visit(TupleInputNode     *node) override;
    llvm::Value* visit(FunctionNode       *node) override;
    llvm::Value* visit(FunctionCallNode   *node) override;
    llvm::Value* visit(TupleNode *node, llvm::StructType * tuple) override;

    //Helper functions
    llvm::StructType* parseStructType(TupleType *node);
	llvm::Value*      performTupleOp(llvm::Value * left, llvm::Value*right, int OPTYPE, int line);
    std::vector<llvm::Value *> getParamVec(std::vector<ASTNode *> *paramNode,std::vector<ASTNode *> *arguNode);
    llvm::Value *getIndexForTuple(ASTNode *index, llvm::Value *tuplePtr);
    llvm::Value *initTuple(int INIT, llvm::StructType *tuple);
    llvm::Function *declareFuncOrProc(std::string functionName, std::string strRetType, std::vector<ASTNode *> *paramsList, int nodeType, int line, TupleType * tupleType = nullptr);
    void generateFuncOrProcBody(llvm::Function *F, std::vector<ASTNode *> *paramsList, ASTNode * block);
    llvm::Value *callFuncOrProc(std::string functionName, std::vector<ASTNode *> *arguments);
    llvm::Value *getPtrToVar(Symbol *idNode, bool constant, std::vector<std::string> &aliasVector,
            llvm::Value *idxVal = nullptr, std::string idxTrueVal = "");

protected:
    llvm::LLVMContext * globalCtx;
    llvm::IRBuilder<> * ir;
    llvm::Module      * mod;
    SymbolTable       * symbolTable;
    CastTable         * ct;
    ErrorBuilder      * eb;

    std::stack<WhileBuilder *> *whileStack;

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
        ct          = new CastTable(globalCtx, ir, it, mod, eb);
        whileStack  = new std::stack<WhileBuilder *>;
        eb          = new ErrorBuilder();
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
