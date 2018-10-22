//
// Created by iali on 10/22/18.
//
#include <AST/ASTNodes/AssignNode.h>
#include <string>

AssignNode::AssignNode(ASTNode *expr, const std::string &ID) : UnaryNode(expr), ID(ID) {}

const std::string &AssignNode::getID() const {
    return ID;
}