//
// Created by lepoidev on 10/26/18.
//

#include <AST/ASTNodes/BaseNodes/ASTNode.h>
#include "AST/ASTNodes/TerminalNodes/RealNode.h"

RealNode::RealNode(float val) : val(val) {
    type = REAL;
}

float RealNode::getVal() const {
    return val;
}