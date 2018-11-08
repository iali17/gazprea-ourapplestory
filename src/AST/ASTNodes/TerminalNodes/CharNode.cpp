//
// Created by lepoidev on 10/26/18.
//
#include <AST/ASTNodes/TerminalNodes/CharNode.h>

CharNode::CharNode(char val, int line) : ASTNode(line), val(val) {
setType(CHAR);}

char CharNode::getVal() const {
    return val;
}
