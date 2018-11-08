//
// Created by kyle on 27/10/18.
//
#include "AST/ASTNodes/TerminalNodes/OutputNode.h"

OutputNode::OutputNode(const std::string &streamName, ASTNode *expr, int line) : IONode(streamName, line), expr(expr) {}

ASTNode *OutputNode::getExpr() const {
    return expr;
}
