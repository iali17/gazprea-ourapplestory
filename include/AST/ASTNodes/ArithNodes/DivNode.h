//
// Created by lepoidev on 10/28/18.
//

#ifndef GAZPREABASE_INTDIVNODE_H
#define GAZPREABASE_INTDIVNODE_H
#include "AST/ASTNodes/BaseNodes/ArithNode.h"

class DivNode : public ArithNode {
public:
    DivNode(ASTNode *left, ASTNode *right, int line);
};
#endif //GAZPREABASE_INTDIVNODE_H
