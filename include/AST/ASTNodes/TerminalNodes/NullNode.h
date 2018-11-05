//
// Created by ali5 on 11/5/18.
//

#ifndef GAZPREABASE_NULLNODE_H
#define GAZPREABASE_NULLNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"
#include <string>
#include <iostream>

class NullNode : public ASTNode {
public:
    NullNode();

    void assignType(std::string newType, int pos);
    void pushType(std::string type);
    std::string popType(int pos);

    int getTypePos(std::string type);

protected:
    std::vector<std::string> *vector;

    std::string typeTable[5][5] = { {"bool", "null", "null", "null"},
                                     {"null", "char", "null", "null"},
                                     {"null", "null", "int", "real"},
                                     {"null", "null", "real", "real"}};
};


#endif //GAZPREABASE_NULLNODE_H
