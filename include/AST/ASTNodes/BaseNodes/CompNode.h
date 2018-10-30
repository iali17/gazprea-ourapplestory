//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_COMPNODE_H
#define GAZPREABASE_COMPNODE_H
#include "InfixNode.h"
class CompNode : public InfixNode {
public:
    CompNode(ASTNode *left, ASTNode *right);
};
#endif //GAZPREABASE_COMPNODE_H
