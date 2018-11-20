//
// Created by iali1 on 11/20/18.
//

#ifndef GAZPREABASE_BUILTIN_H
#define GAZPREABASE_BUILTIN_H

#include "ASTNode.h"
#include <string>

class BuiltIn : public ASTNode {
public:
    BuiltIn(int line, const std::string &ID);
    const std::string &getID() const;

protected:
    std::string ID;
};

#endif //GAZPREABASE_BUILTIN_H