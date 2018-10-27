//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTBASEVISITOR_H
#define GAZPREABASE_ASTBASEVISITOR_H

#include <AST/ASTNodes/TerminalNodes/INTNode.h>
#include "ASTVisitor.h"


class ASTBaseVisitor : public ASTVisitor {
public:
    llvm::Value* visit(ASTNode   *node) override {

        if      (dynamic_cast<FileNode *>(node)) {
            return visit((FileNode *) node);
        }
        else if (dynamic_cast<BasicBlockNode *>(node)) {
            return visit((BasicBlockNode *) node);
        }
        else if (dynamic_cast<BlockNode *>(node)) {
            return visit((BlockNode *) node);
        }
        else if (dynamic_cast<ProcedureNode *>(node)) {
            return visit((ProcedureNode *) node);
        }
        else if (dynamic_cast<ParamNode *>(node)) {
            return visit((ParamNode *) node);
        }
        else if (dynamic_cast<ReturnNode *>(node)) {
            return visit((ReturnNode *) node);
        }
        else if (dynamic_cast<INTNode *>(node)) {
            return visit((INTNode *) node);
        }
        else if (dynamic_cast<RealNode *>(node)) {
            return visit((RealNode *) node);
        }
        else if (dynamic_cast<CharNode *>(node)) {
            return visit((CharNode *) node);
        }
        else if (dynamic_cast<BoolNode *>(node)) {
            return visit((BoolNode *) node);
        }
        else if (dynamic_cast<CondNode *>(node)) {
            return visit((CondNode *) node);
        }
        fprintf(stderr, "Unrecognized class\n");
        return nullptr;
    };

    llvm::Value* visit(FileNode  *node) override { return nullptr;};
    llvm::Value* visit(BasicBlockNode *node) override { return nullptr;};
    llvm::Value* visit(BlockNode *node) override { return nullptr;};
    llvm::Value* visit(ProcedureNode  *node) override { return nullptr;};
    llvm::Value* visit(ParamNode *node) override { return nullptr;};
    llvm::Value* visit(ReturnNode *node) override { return nullptr;};
    llvm::Value* visit(INTNode *node) override { return nullptr;};
    llvm::Value* visit(RealNode *node) override { return nullptr;};
    llvm::Value* visit(CharNode *node) override { return nullptr;};
    llvm::Value* visit(BoolNode *node) override { return nullptr;};
    llvm::Value* visit(CondNode *node) override { return nullptr;};
};

#endif //GAZPREABASE_ASTBASEVISITOR_H
