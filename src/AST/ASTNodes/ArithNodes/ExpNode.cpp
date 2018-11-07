//
// Created by ali5 on 11/2/18.
//

#include "AST/AST.h"

ExpNode::ExpNode(ASTNode *left, ASTNode *right, int line) : ArithNode(left, right), line(line) {}

int ExpNode::getLine() {
    return this->line;
}