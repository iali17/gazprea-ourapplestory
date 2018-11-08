//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_INFIXNODES_H
#define GAZPREABASE_INFIXNODES_H
#include "ASTNode.h"

class InfixNode : public ASTNode{
public:
    InfixNode(ASTNode *left, ASTNode *right, int line);
    ASTNode *getLeft() const;
    ASTNode *getRight() const;

protected:
    ASTNode *left, *right;
};

#endif //GAZPREABASE_INFIXNODES_H
