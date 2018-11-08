//
// Created by iali on 10/22/18.
//
#include <AST/ASTNodes/StatementNodes/AssignNode.h>
#include <string>

AssignNode::AssignNode(ASTNode *expr, const std::string &ID, int line) : UnaryNode(expr, line), ID(ID) {}

const std::string &AssignNode::getID() const {
    return ID;
}