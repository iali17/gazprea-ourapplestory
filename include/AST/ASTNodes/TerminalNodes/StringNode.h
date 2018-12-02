//
// Created by iali1 on 12/2/18.
//

#ifndef GAZPREABASE_STRINGNODE_H
#define GAZPREABASE_STRINGNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class StringNode : public ASTNode {
public:
    StringNode(std::vector<ASTNode *> *elements, int line);
    std::vector<ASTNode *>* getElements();

protected:
    std::vector<ASTNode *> *elements;
};

#endif //GAZPREABASE_STRINGNODE_H
