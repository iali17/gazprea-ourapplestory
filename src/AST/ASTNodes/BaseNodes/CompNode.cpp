//
// Created by kyle on 29/10/18.
//

#include "AST/ASTNodes/BaseNodes/CompNode.h"

CompNode::CompNode(ASTNode *left, ASTNode *right) : InfixNode(left, right) {}