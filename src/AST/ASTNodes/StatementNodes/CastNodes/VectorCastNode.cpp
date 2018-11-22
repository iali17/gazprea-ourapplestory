//
// Created by andyli on 21/11/18.
//

#include "AST/ASTNodes/StatementNodes/CastNodes/VectorCastNode.h"

VectorCastNode::VectorCastNode(ASTNode *expr, ASTNode *vector, int line) : CastExprNode(expr, VECTOR_CAST, line), vector(vector){}

ASTNode* VectorCastNode::getVector() {
    return this->vector;
}