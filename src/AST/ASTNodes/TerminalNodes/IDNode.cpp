//
// Created by iali on 10/22/18.
//

#include <AST/ASTNodes/TerminalNodes/IDNode.h>
#include <string>

IDNode::IDNode() = default;

const std::string &IDNode::getID() const {
    return this->ID;
}

IDNode::IDNode(int type, const std::string &ID) : ID(ID) {setType(type); }

IDNode::IDNode(const std::string &ID) : ID(ID) {}
