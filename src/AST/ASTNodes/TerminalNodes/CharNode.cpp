//
// Created by lepoidev on 10/26/18.
//

#include <AST/ASTNodes/TerminalNodes/CharNode.h>

CharNode::CharNode(char val) : val(val) {
    type = CHAR;
}

char CharNode::getVal() const {
    return val;
}
