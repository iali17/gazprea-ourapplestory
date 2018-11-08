//
// Created by kyle on 26/10/18.
//
#include "AST/ASTNodes/StatementNodes/LoopNodes/InLoopNode.h"

InLoopNode::InLoopNode(ASTNode *body, ASTNode *control, int line) : BaseLoopNode((BlockNode *) body, control, line) {}