//
// Created by ali5 on 11/20/18.
//

#include "AST/ASTNodes/TypeNodes/VectorType.h"

VectorType::VectorType(ASTNode *size, int line) : size(size), ASTNode(line){}

ASTNode* VectorType::getSize() const {
    return this->size;
}