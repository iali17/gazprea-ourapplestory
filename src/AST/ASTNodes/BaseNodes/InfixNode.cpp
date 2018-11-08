//
// Created by kyle on 20/10/18.
//
#include <AST/ASTNodes/BaseNodes/InfixNode.h>

InfixNode::InfixNode(ASTNode *left, ASTNode *right, int line) : ASTNode(line), left(left), right(right) {}

ASTNode *InfixNode::getLeft() const {
    return left;
}

ASTNode *InfixNode::getRight() const {
    return right;
}
