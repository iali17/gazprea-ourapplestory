//
// Created by csun on 11/20/18.
//

#include <AST/ASTNodes/TypeNodes/MatrixType.h>

#include "AST/ASTNodes/TypeNodes/MatrixType.h"



MatrixType::MatrixType(ASTNode *left, ASTNode *right, int line) : ASTNode(line), left(left), right(right) {
}

ASTNode *MatrixType::getLeft() const {
    return left;
}

ASTNode *MatrixType::getRight() const {
    return right;
}






