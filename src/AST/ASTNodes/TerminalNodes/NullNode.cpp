//
// Created by ali5 on 11/5/18.
//
#include "AST/ASTNodes/TerminalNodes/NullNode.h"

NullNode::NullNode() {
    setType(NULLTY);

    this->vector = new std::vector<std::string>;
    this->vector->push_back("null");
}

void NullNode::pushType(std::string type) {
    this->vector->push_back(type);
}

std::string NullNode::popType(int pos) {
    return this->vector->at(pos);
}

void NullNode::assignType(std::string newType, int pos) {
    int curTypePos = this->getTypePos(this->vector->at(pos));
    int newTypePos = this->getTypePos(newType);

    std::string varType = typeTable[curTypePos][newTypePos];

    this->vector->at(pos) = varType;
}

int NullNode::getTypePos(std::string type) {
    if(type == "bool")
        return 0;
    else if(type == "char")
        return 1;
    else if(type == "int")
        return 2;
    else if(type == "real")
        return 3;
    else if(type == "null")
        return 4;
    else {
        std::cerr << "Unknown type \n";
        exit(1);
    }
}