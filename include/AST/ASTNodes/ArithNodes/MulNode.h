//
// Created by lepoidev on 10/28/18.
//

#ifndef GAZPREABASE_INTMULNODE_H
#define GAZPREABASE_INTMULNODE_H
#include "AST/ASTNodes/BaseNodes/ArithNode.h"

class MulNode : public ArithNode {
public:
    MulNode(ASTNode *left, ASTNode *right, int line);

    int getLine();

protected:
    int line;
};
#endif //GAZPREABASE_INTMULNODE_H
