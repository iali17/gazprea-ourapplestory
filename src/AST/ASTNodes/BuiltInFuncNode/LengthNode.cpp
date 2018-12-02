//
// Created by kyle on 26/11/18.
//

#include "AST/AST.h"

LengthNode::LengthNode(ASTNode *expr, int line) : UnaryNode(expr, line) {
    setType(INTEGER);
}
