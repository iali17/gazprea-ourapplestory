//
// Created by lepoidev on 10/28/18.
//

#include "AST/AST.h"

AddNode::AddNode(ASTNode *left, ASTNode *right, int line) : ArithNode(left, right, line) {
}
