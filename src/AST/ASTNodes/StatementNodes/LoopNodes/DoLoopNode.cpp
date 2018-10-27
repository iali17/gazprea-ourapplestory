//
// Created by kyle on 26/10/18.
//

#include "AST/ASTNodes/StatementNodes/LoopNodes/DoLoopNode.h"

DoLoopNode::DoLoopNode(ASTNode *body, ASTNode *control) : BaseLoopNode((BlockNode *) body, control) {}
