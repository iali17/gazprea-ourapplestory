//
// Created by ali5 on 11/6/18.
//
#include "AST/ASTNodes/TerminalNodes/TypeDefNode.h"

TypeDefNode::TypeDefNode(const std::string &id, const std::string &type): id(id), type(type){}

TypeDefNode::TypeDefNode(const std::string &id, ASTNode *tupleType): id(id), tupleType(tupleType) {}

const std::string &TypeDefNode::getCustomType() const{
    return this->type;
}

const std::string &TypeDefNode::getId() const{
    return this->id;
}

ASTNode* TypeDefNode::getTuple() {
    return this->tupleType;
}