//
// Created by kyle on 29/10/18.
//
#include "AST/AST.h"

NegateNode::NegateNode(ASTNode *expr, int line) : UnaryNode(expr, line) {}
