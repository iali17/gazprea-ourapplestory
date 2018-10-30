//
// Created by lepoidev on 10/28/18.
//

#ifndef GAZPREABASE_REMNODE_H
#define GAZPREABASE_REMNODE_H
#include "AST/ASTNodes/BaseNodes/ArithNode.h"

class RemNode : public ArithNode {
public:
    RemNode(ASTNode *left, ASTNode *right);
};

#endif //GAZPREABASE_REMNODE_H
