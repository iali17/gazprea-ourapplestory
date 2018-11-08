//
// Created by lepoidev on 10/26/18.
//
#include <AST/ASTNodes/BaseNodes/ASTNode.h>
#include <AST/ASTNodes/TerminalNodes/RealNode.h>

RealNode::RealNode(float val, int line) : ASTNode(line), val(val) { setType(REAL); }

float RealNode::getVal() const {
    return val;
}