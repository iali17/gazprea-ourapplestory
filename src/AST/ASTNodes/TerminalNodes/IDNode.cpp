//
// Created by iali on 10/22/18.
//
#include <AST/ASTNodes/TerminalNodes/IDNode.h>
#include <string>


const std::string &IDNode::getID() const {
    return this->ID;
}

IDNode::IDNode(int type, const std::string &ID, int line) : ASTNode(line), ID(ID) {setType(type); }

IDNode::IDNode(const std::string &ID, int line) : ASTNode(line), ID(ID) {}

IDNode::IDNode(int line) : ASTNode(line) {}
