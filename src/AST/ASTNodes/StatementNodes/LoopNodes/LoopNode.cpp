//
// Created by kyle on 26/10/18.
//

#include "AST/ASTNodes/StatementNodes/LoopNodes/LoopNode.h"

LoopNode::LoopNode(ASTNode *body) : BaseLoopNode(body) {
    control = nullptr;
}

LoopNode::LoopNode(ASTNode *body, ASTNode *control) : BaseLoopNode((BlockNode *) body, control) {}
