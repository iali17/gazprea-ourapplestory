//
// Created by kyle on 29/10/18.
//
#include "AST/AST.h"

EQNode::EQNode(ASTNode *left, ASTNode *right, int line) : CompNode(left, right), line(line) {}

int EQNode::getLine() {
    return this->line;
}