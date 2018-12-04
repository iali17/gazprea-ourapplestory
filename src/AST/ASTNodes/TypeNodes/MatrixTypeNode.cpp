//
// Created by csun on 11/20/18.
//

#include <AST/ASTNodes/TypeNodes/MatrixTypeNode.h>

#include "AST/ASTNodes/TypeNodes/MatrixTypeNode.h"



MatrixTypeNode::MatrixTypeNode(ASTNode *left, ASTNode *right, int line, const std::string &stringType) : ASTNode(line), left(left), right(right), stringType(stringType)  {
}

ASTNode *MatrixTypeNode::getLeft() const {
    return left;
}

ASTNode *MatrixTypeNode::getRight() const {
    return right;
}

const std::string& MatrixTypeNode::getStringType() const {
    return this->stringType;
}


