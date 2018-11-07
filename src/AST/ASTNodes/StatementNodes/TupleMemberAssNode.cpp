//
// Created by iali1 on 11/6/18.
//

#include <AST/ASTNodes/StatementNodes/TupleMemberAssNode.h>

TupleMemberAssNode::TupleMemberAssNode(ASTNode *expr, IndexTupleNode *LHS) : UnaryNode(expr), LHS(LHS) {}

IndexTupleNode *TupleMemberAssNode::getLHS() const {
    return LHS;
}