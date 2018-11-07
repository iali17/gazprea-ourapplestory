//
// Created by ali5 on 11/2/18.
//

#ifndef GAZPREABASE_EXPNODE_H
#define GAZPREABASE_EXPNODE_H

#include "AST/ASTNodes/BaseNodes/ArithNode.h"

class ExpNode : public ArithNode {
public:
    ExpNode(ASTNode *left, ASTNode *right, int line);

    int getLine();

protected:
    int line;
};

#endif //GAZPREABASE_EXPNODE_H
