//
// Created by kyle on 26/10/18.
//
#include "AST/ASTNodes/StatementNodes/LoopNodes/LoopNode.h"

LoopNode::LoopNode(ASTNode *body, int line) : BaseLoopNode(body), line(line) {
    control = nullptr;
}

LoopNode::LoopNode(ASTNode *body, ASTNode *control, int line) : BaseLoopNode((BlockNode *) body, control), line(line) {}

int LoopNode::getLine() {
    return this->line;
}