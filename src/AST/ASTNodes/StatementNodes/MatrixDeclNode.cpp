//
// Created by csun on 11/20/18.
//

#include "AST/ASTNodes/StatementNodes/MatrixDeclNode.h"

MatrixDeclNode::MatrixDeclNode(ASTNode *expr, int line, bool constant, const std::string &ID,
        const std::string &initType, ASTNode *matrixType)
        : UnaryNode(expr, line), constant(constant), ID(ID), initType(initType), matrixType(matrixType) {}


bool MatrixDeclNode::isConstant() const {
    return constant;
}

const std::string &MatrixDeclNode::getID() const {
    return ID;
}

const std::string &MatrixDeclNode::getInitType() const {
    return initType;
}

ASTNode *MatrixDeclNode::getMatrixType() const {
    return matrixType;
}




