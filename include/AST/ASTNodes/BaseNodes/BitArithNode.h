//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_BITARITHNODE_H
#define GAZPREABASE_BITARITHNODE_H
#include "AST/ASTNodes/BaseNodes/InfixNode.h"

class BitArithNode : public InfixNode {
public:
    BitArithNode(ASTNode *left, ASTNode *right);
};
#endif //GAZPREABASE_BITARITHNODE_H
