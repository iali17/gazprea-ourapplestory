//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTBASEVISITOR_H
#define GAZPREABASE_ASTBASEVISITOR_H

#include "ASTVisitor.h"


class ASTBaseVisitor : public ASTVisitor {
public:
    llvm::Value* visit(ASTNode   *node) {
        if      (dynamic_cast<FileNode *>(node)) {
            return visit((FileNode *) node);
        }
        else if (dynamic_cast<BasicBlockNode *>(node)) {
            return visit((BasicBlockNode *) node);
        }
        else if (dynamic_cast<BlockNode *>(node)) {
            return visit((BlockNode *) node);
        }
        return nullptr;
    };

    llvm::Value* visit(FileNode  *node) { return nullptr;};
    llvm::Value* visit(BasicBlockNode *node) { return nullptr;};
    llvm::Value* visit(BlockNode *node) { return nullptr;};
};

#endif //GAZPREABASE_ASTBASEVISITOR_H
