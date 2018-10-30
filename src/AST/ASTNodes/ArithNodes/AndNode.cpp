//
// Created by kyle on 29/10/18.
//

#include "AST/AST.h"

AndNode::AndNode(ASTNode *left, ASTNode *right) : BitArithNode(left, right) {}
