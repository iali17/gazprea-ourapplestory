//
// Created by ali5 on 11/20/18.
//

#include "AST/ASTNodes/StatementNodes/VectorDeclNode.h"

VectorDeclNode::VectorDeclNode(ASTNode *expr, bool constant, const std::string &ID, const std::string &stringType, ASTNode *vectorType, int line)
: UnaryNode(expr, line), constant(constant), ID(ID), stringType(stringType), vectorType(vectorType){}

const std::string& VectorDeclNode::getID() const {
    return this->ID;
}

bool VectorDeclNode::isConstant() const {
    return this->constant;
}

ASTNode* VectorDeclNode::getVectorType() {
    return  this->vectorType;
}

const std::string& VectorDeclNode::getStringType() const {
    return this->stringType;
}