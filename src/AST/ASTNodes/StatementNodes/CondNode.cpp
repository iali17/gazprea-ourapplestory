//
// Created by kyle on 26/10/18.
//
#include "AST/ASTNodes/StatementNodes/CondNode.h"

CondNode::CondNode(std::vector<ASTNode *> *conds, std::vector<ASTNode *> *blocks, int line) : ASTNode(line), conds(conds), blocks(blocks) {
    hasElse = blocks->size() > conds->size();
}

std::vector<ASTNode *> *CondNode::getConds() const {
    return conds;
}

std::vector<ASTNode *> *CondNode::getBlocks() const {
    return blocks;
}

bool CondNode::isHasElse() const {
    return hasElse;
}
