//
// Created by kyle on 29/10/18.
//

#include "AST/AST.h"

GTNode::GTNode(ASTNode *left, ASTNode *right, int line) : CompNode(left, right), line(line) {}

int GTNode::getLine() {
    return this->line;
}