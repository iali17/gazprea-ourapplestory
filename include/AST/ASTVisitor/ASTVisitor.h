//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTVISITOR_H
#define GAZPREABASE_ASTVISITOR_H

#include <llvm/IR/Value.h>
#include <AST/ASTNodes/FuncProcNodes/CallNode.h>
#include <AST/ASTNodes/FuncProcNodes/ProcedureCallNode.h>
#include <AST/ASTNodes/TerminalNodes/TupleNode.h>
#include <AST/ASTNodes/StatementNodes/TupleDeclNode.h>
#include <AST/ASTNodes/TypeNodes/TupleType.h>
#include "../AST.h"

class ASTVisitor {
public:
    virtual llvm::Value* visit(ASTNode   *node)      = 0;
    virtual llvm::Value* visit(FileNode  *node)      = 0;
    virtual llvm::Value* visit(BasicBlockNode *node) = 0;
    virtual llvm::Value* visit(BlockNode *node)      = 0;
    virtual llvm::Value* visit(ProcedureNode  *node) = 0;
    virtual llvm::Value* visit(ParamNode *node)      = 0;
    virtual llvm::Value* visit(ReturnNode *node)     = 0;
    virtual llvm::Value* visit(INTNode *node)        = 0;
    virtual llvm::Value* visit(RealNode *node)       = 0;
    virtual llvm::Value* visit(CharNode *node)       = 0;
    virtual llvm::Value* visit(BoolNode *node)       = 0;
    virtual llvm::Value* visit(CondNode *node)       = 0;
    virtual llvm::Value* visit(DoLoopNode *node)     = 0;
    virtual llvm::Value* visit(LoopNode *node)       = 0;
    virtual llvm::Value* visit(InLoopNode *node)     = 0;
    virtual llvm::Value* visit(AssignNode *node)     = 0;
    virtual llvm::Value* visit(DeclNode *node)       = 0;
    virtual llvm::Value* visit(CastExprNode *node)   = 0;
    virtual llvm::Value* visit(IDNode *node)         = 0;
    virtual llvm::Value* visit(InputNode *node)      = 0;
    virtual llvm::Value* visit(OutputNode *node)     = 0;
    virtual llvm::Value* visit(AddNode *node)        = 0;
    virtual llvm::Value* visit(SubNode *node)        = 0;
    virtual llvm::Value* visit(MulNode *node)        = 0;
    virtual llvm::Value* visit(DivNode *node)        = 0;
    virtual llvm::Value* visit(RemNode *node)        = 0;
    virtual llvm::Value* visit(ExpNode *node)        = 0;
    virtual llvm::Value* visit(StreamDeclNode *node) = 0;
    virtual llvm::Value* visit(CallNode *node)       = 0;
    virtual llvm::Value* visit(EQNode *node)         = 0;
    virtual llvm::Value* visit(NEQNode *node)        = 0;
    virtual llvm::Value* visit(GTNode *node)         = 0;
    virtual llvm::Value* visit(LTNode *node)         = 0;
    virtual llvm::Value* visit(AndNode *node)        = 0;
    virtual llvm::Value* visit(OrNode *node)         = 0;
    virtual llvm::Value* visit(XOrNode *node)        = 0;
    virtual llvm::Value* visit(NegateNode *node)     = 0;
    virtual llvm::Value* visit(GTENode *node)        = 0;
    virtual llvm::Value* visit(LTENode *node)        = 0;
    virtual llvm::Value* visit(ContinueNode *node)   = 0;
    virtual llvm::Value* visit(BreakNode *node)      = 0;
    virtual llvm::Value* visit(ProcedureCallNode *node)      = 0;
    virtual llvm::Value* visit(TupleDeclNode *node)  = 0;
    virtual llvm::Value* visit(TupleType *node)      = 0;

};

#endif //GAZPREABASE_ASTVISITOR_H