//
// Created by kyle on 04/11/18.
//
#include "AST/AST.h"

GlobalDeclNode::GlobalDeclNode(ASTNode *expr, const std::string &ID, std::vector<std::string> *typeIds) : UnaryNode(
        expr), ID(ID), typeIds(typeIds) {}

const std::string &GlobalDeclNode::getID() const {
    return ID;
}

std::vector<std::string> *GlobalDeclNode::getTypeIds() const {
    return typeIds;
}
