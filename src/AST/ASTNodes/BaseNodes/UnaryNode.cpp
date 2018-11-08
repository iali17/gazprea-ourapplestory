//
// Created by kyle on 20/10/18.
//
#include <AST/ASTNodes/BaseNodes/UnaryNode.h>

UnaryNode::UnaryNode(ASTNode *expr, int line) : ASTNode(line), expr(expr) {}

ASTNode *UnaryNode::getExpr() const {
    return expr;
}
