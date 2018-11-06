//
// Created by ali5 on 11/5/18.
//

#ifndef GAZPREABASE_IDNNODE_H
#define GAZPREABASE_IDNNODE_H

#include <iostream>
#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class IdnNode : public ASTNode {
public:
    IdnNode();
    void assignType(std::string newType, int pos);
    void pushType(std::string type);
    std::string popType(int pos);

    int getTypePos(std::string type);

protected:
    std::vector<std::string> *vector;

    std::string typeTable[5][5] = { {"bool", "idn", "idn", "idn", "bool"},
                                    {"idn", "char", "idn", "idn", "char"},
                                    {"idn", "idn", "int",  "real", "int" },
                                    {"idn", "idn", "real", "real", "real"},
                                    {"bool", "char", "int",  "real", "idn"}};
    
};

#endif //GAZPREABASE_IDNNODE_H
