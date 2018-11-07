//
// Created by lepoidev on 10/28/18.
//

#ifndef GAZPREABASE_INTSUBNODE_H
#define GAZPREABASE_INTSUBNODE_H
#include "AST/ASTNodes/BaseNodes/ArithNode.h"
class SubNode : public ArithNode {
public:
    SubNode(ASTNode *left, ASTNode *right, int line);

    int getLine();

protected:
    int line;
};

#endif //GAZPREABASE_INTSUBNODE_H
