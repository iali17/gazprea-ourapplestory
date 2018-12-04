//
// Created by ali5 on 11/20/18.
//

#include "AST/ASTNodes/TypeNodes/VectorTypeNode.h"

VectorTypeNode::VectorTypeNode(ASTNode *size, const std::string &stringType, int line) : ASTNode(line), size(size), stringType(stringType){}

ASTNode* VectorTypeNode::getSize() const {
    return this->size;
}

const std::string& VectorTypeNode::getStringType() const {
    return this->stringType;
}