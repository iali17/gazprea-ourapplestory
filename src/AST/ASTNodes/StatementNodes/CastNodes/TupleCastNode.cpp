//
// Created by andyli on 21/11/18.
//

#include "AST/ASTNodes/StatementNodes/CastNodes/TupleCastNode.h"

TupleCastNode::TupleCastNode(ASTNode *expr, ASTNode *tuple, int line): CastExprNode(expr, TUPLE_CAST, line), tuple(tuple) {}


ASTNode* TupleCastNode::getTuple() {
    return this->tuple;
}