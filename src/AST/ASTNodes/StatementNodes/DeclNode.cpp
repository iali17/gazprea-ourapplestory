//
// Created by iali on 10/22/18.
//

#include <AST/ASTNodes/StatementNodes/DeclNode.h>
#include <string>

const std::string &DeclNode::getID() const {
    return ID;
}

DeclNode::DeclNode(ASTNode *expr, bool constant, const std::string &ID, std::vector<std::string> *typeIds, int newType) : UnaryNode(
        expr), constant(constant), ID(ID), typeIds(typeIds) {setType(newType)}

bool DeclNode::isConstant() const {
    return constant;
}

std::vector<std::string> *DeclNode::getTypeIds() const {
    return typeIds;
}
