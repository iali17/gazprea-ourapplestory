//
// Created by kyle on 29/10/18.
//

#include "AST/AST.h"

GTENode::GTENode(ASTNode *left, ASTNode *right, int line) : CompNode(left, right), line(line) {}

int GTENode::getLine() {
    return this->line;
}