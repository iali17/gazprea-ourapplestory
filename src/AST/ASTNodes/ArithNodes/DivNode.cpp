//
// Created by lepoidev on 10/28/18.
//

#include "AST/AST.h"

DivNode::DivNode(ASTNode *left, ASTNode *right, int line) : ArithNode(left, right), line(line) {}

int DivNode::getLine() {
    return this->line;
}