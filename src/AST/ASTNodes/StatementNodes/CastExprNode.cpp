//
// Created by ali5 on 10/30/18.
//
#include "AST/ASTNodes/StatementNodes/CastExprNode.h"

CastExprNode::CastExprNode(ASTNode *expr, const std::string &type, int line) : UnaryNode(expr, line), type(type) {}

CastExprNode::CastExprNode(ASTNode *expr, ASTNode *tuple, int line): UnaryNode(expr, line), tuple(tuple) {}

const std::string &CastExprNode::getTypeString() const {
    return type;
}

ASTNode* CastExprNode::getTuple() {
    return this->tuple;
}