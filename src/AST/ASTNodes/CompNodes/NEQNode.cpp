//
// Created by kyle on 29/10/18.
//

#include "AST/AST.h"

NEQNode::NEQNode(ASTNode *left, ASTNode *right, int line) : CompNode(left, right), line(line) {}

int NEQNode::getLine() {
    return this->line;
}