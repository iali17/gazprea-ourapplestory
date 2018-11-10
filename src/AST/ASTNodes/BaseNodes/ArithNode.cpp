//
// Created by kyle on 29/10/18.
//

#include "AST/AST.h"

ArithNode::ArithNode(ASTNode *left, ASTNode *right, int line) : InfixNode(left, right, line) {
    setType(left->getType() > right->getType() ? left->getType(): right->getType());
}