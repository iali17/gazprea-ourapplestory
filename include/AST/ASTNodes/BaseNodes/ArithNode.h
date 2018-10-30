//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_ARITHNODE_H
#define GAZPREABASE_ARITHNODE_H
#include "InfixNode.h"

class ArithNode : public InfixNode {
public:
    ArithNode(ASTNode *left, ASTNode *right);
};

#endif //GAZPREABASE_ARITHNODE_H
