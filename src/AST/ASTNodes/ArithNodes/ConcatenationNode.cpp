//
// Created by kyle on 25/11/18.
//

#include "AST/AST.h"

ConcatenationNode::ConcatenationNode(ASTNode *left, ASTNode *right, int line) : InfixNode(left, right, line) {
    setType((left->getType() > right->getType()) ? left->getType() : right->getType());
}
