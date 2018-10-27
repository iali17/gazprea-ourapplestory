//
// Created by iali on 10/22/18.
//

#include <AST/ASTNodes/StatementNodes/DeclNode.h>
#include <string>

DeclNode::DeclNode(ASTNode *expr, const std::string &ID,
        const std::string &type_id, int newType)
        : UnaryNode(expr), ID(ID), type_id(type_id) {setType(newType);}

const std::string &DeclNode::getID() const {
    return ID;
}