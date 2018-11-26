//
// Created by kyle on 25/11/18.
//

#ifndef GAZPREABASE_DOTPRODUCTNODE_H
#define GAZPREABASE_DOTPRODUCTNODE_H

#include "AST/ASTNodes/BaseNodes/InfixNode.h"

class DotProductNode : public InfixNode {
public:
    DotProductNode(ASTNode *left, ASTNode *right, int line);
};

#endif //GAZPREABASE_DOTPRODUCTNODE_H
