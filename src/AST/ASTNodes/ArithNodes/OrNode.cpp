//
// Created by kyle on 29/10/18.
//

#include "AST/AST.h"

OrNode::OrNode(ASTNode *left, ASTNode *right) : BitArithNode(left, right) {}
