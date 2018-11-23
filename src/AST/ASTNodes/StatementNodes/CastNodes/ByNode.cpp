//
// Created by csun on 11/22/18.
//

#include <AST/ASTNodes/StatementNodes/CastNodes/ByNode.h>

#include "AST/ASTNodes/StatementNodes/CastNodes/ByNode.h"

ByNode::ByNode(ASTNode *left, ASTNode *right, int line) : InfixNode(left, right, line) {

}
