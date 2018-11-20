//
// Created by ali5 on 11/20/18.
//

#include "AST/ASTNodes/StatementNodes/VectorDeclNode.h"

VectorDeclNode::VectorDeclNode(ASTNode *expr, bool constant, const std::string &ID, int line) : UnaryNode(expr, line), constant(constant), ID(ID) {}

VectorDeclNode::VectorDeclNode(ASTNode *expr, bool constant, const std::string &ID, ASTNode *size, int line) : UnaryNode(expr, line), constant(constant), ID(ID), size(size){}

const std::string& VectorDeclNode::getID() const {
    return this->ID;
}

bool VectorDeclNode::isConstant() const {
    return this->constant;
}

ASTNode* VectorDeclNode::getSize() {
    return  this->size;
}