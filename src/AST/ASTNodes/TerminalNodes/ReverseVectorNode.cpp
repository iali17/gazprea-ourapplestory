//
// Created by kyle on 22/11/18.
//
#include "AST/AST.h"

ReverseVectorNode::ReverseVectorNode(ASTNode *expr, int line) : UnaryNode(expr, line) {
    setType(VECTOR_T);
}
