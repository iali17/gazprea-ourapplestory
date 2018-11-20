//
// Created by iali1 on 11/20/18.
//

#include <AST/ASTNodes/BaseNodes/BuiltIn.h>

const std::string &BuiltIn::getID() const {
    return ID;
}

BuiltIn::BuiltIn(int line, const std::string &ID) : ASTNode(line), ID(ID){ }
