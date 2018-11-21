//
// Created by ali5 on 11/20/18.
//

#include "AST/ASTNodes/TypeNodes/VectorType.h"

VectorType::VectorType(ASTNode *size, int line) :ASTNode(line),  size(size){}

VectorType::VectorType(ASTNode *size, const std::string &stringType, int line) : ASTNode(line), size(size), stringType(stringType){}

ASTNode* VectorType::getSize() const {
    return this->size;
}

const std::string& VectorType::getStringType() const {
    return this->stringType;
}