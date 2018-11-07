//
// Created by ali5 on 10/30/18.
//
#include "AST/ASTNodes/StatementNodes/CastExprNode.h"

CastExprNode::CastExprNode(ASTNode *expr, const std::string &type, int line) : UnaryNode(expr), type(type), line(line){}

const std::string &CastExprNode::getTypeString() const {
    return type;
}

int CastExprNode::getLine() {
    return this->line;
}