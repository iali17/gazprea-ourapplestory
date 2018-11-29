//
// Created by csun on 11/20/18.
//

#include <AST/ASTNodes/TypeNodes/MatrixTypeNode.h>

#include "AST/ASTNodes/TypeNodes/MatrixTypeNode.h"



MatrixTypeNode::MatrixTypeNode(ASTNode *left, ASTNode *right, int line) : ASTNode(line), left(left), right(right) {
}

ASTNode *MatrixTypeNode::getLeft() const {
    return left;
}

ASTNode *MatrixTypeNode::getRight() const {
    return right;
}






