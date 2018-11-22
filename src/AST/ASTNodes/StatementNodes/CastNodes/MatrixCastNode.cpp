//
// Created by andyli on 21/11/18.
//

#include "AST/ASTNodes/StatementNodes/CastNodes/MatrixCastNode.h"

MatrixCastNode::MatrixCastNode(ASTNode *expr, ASTNode *matrix, int line) : CastExprNode(expr, MATRIX_CAST, line), matrix(matrix){}

ASTNode* MatrixCastNode::getMatrix() {
    return this->matrix;
}