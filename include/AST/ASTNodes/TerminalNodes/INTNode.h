//
// Created by iali on 10/22/18.
//

#ifndef GAZPREABASE_INTNODE_H
#define GAZPREABASE_INTNODE_H
#include "AST/ASTNodes/BaseNodes/ASTNode.h"

//TODO: should the value be protected and we just have a getVal?
class INTNode : public ASTNode {
public:
    explicit INTNode(int value, int line);
    int value;
};

#endif //GAZPREABASE_INTNODE_H
