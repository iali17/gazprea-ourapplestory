//
// Created by lepoidev on 10/26/18.
//
#include "AST/ASTNodes/TerminalNodes/BoolNode.h"

bool BoolNode::getVal() const {
    return val;
}

BoolNode::BoolNode(bool val, int line) : val(val), ASTNode(line) {setType(BOOLEAN);}
