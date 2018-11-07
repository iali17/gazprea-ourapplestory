//
// Created by lepoidev on 10/28/18.
//

#include "AST/AST.h"

SubNode::SubNode(ASTNode *left, ASTNode *right, int line) : ArithNode(left, right), line(line) {}

int SubNode::getLine() {
    return this->line;
}