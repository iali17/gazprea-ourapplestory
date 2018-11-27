//
// Created by kyle on 26/11/18.
//

#include <unordered_set>
#include "AST/AST.h"

SliceAssignNode::SliceAssignNode(ASTNode *left, ASTNode *right, int line) : InfixNode(left, right, line) {}
