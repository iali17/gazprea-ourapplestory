//
// Created by ali5 on 10/30/18.
//
#include "AST/ASTNodes/StatementNodes/CastNodes/CastExprNode.h"

CastExprNode::CastExprNode(ASTNode *expr, castType cType, int line) : UnaryNode(expr, line), cType(cType){}

CastExprNode::castType CastExprNode::getCastType() const {
    return cType;
}