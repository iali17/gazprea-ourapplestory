//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTVISITOR_H
#define GAZPREABASE_ASTVISITOR_H

#include <llvm/IR/Value.h>
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
};

#endif //GAZPREABASE_ASTVISITOR_H
