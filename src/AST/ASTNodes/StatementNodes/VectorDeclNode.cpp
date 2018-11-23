//
// Created by ali5 on 11/20/18.
//

#include "AST/ASTNodes/StatementNodes/VectorDeclNode.h"

VectorDeclNode::VectorDeclNode(ASTNode *expr, bool constant, const std::string &ID, ASTNode *vectorType,
        ASTNode *size, int line) : UnaryNode(expr, line), constant(constant), ID(ID),  vectorType(vectorType), size(size){}

const std::string& VectorDeclNode::getID() const {
    return this->ID;
}

bool VectorDeclNode::isConstant() const {
    return this->constant;
}

ASTNode* VectorDeclNode::getVectorType() {
    return  this->vectorType;
}

ASTNode *VectorDeclNode::getSize() const {
    return this->size;
}
