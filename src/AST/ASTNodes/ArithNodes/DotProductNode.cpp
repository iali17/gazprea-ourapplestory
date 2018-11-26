//
// Created by kyle on 25/11/18.
//

#include "AST/AST.h"

DotProductNode::DotProductNode(ASTNode *left, ASTNode *right, int line) : InfixNode(left, right, line) {}
