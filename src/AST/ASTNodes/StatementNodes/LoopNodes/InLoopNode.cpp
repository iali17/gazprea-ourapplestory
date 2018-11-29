//
// Created by kyle on 26/10/18.
//
#include "AST/ASTNodes/StatementNodes/LoopNodes/InLoopNode.h"

InLoopNode::InLoopNode(BlockNode *blockNode, ASTNode *control, int line, ASTNode *domain, const std::string &loopVar)
        : BaseLoopNode(blockNode, control, line), domain(domain), loopVar(loopVar) {}

ASTNode *InLoopNode::getDomain() const {
    return domain;
}

const std::string &InLoopNode::getLoopVar() const {
    return loopVar;
}
