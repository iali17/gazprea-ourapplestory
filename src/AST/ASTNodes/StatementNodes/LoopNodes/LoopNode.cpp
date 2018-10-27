//
// Created by kyle on 26/10/18.
//

#include "AST/ASTNodes/StatementNodes/LoopNodes/LoopNode.h"

LoopNode::LoopNode(ASTNode *body) : BaseLoopNode(body) {}

LoopNode::LoopNode(ASTNode *body, ASTNode *control) : BaseLoopNode(body, control) {}
