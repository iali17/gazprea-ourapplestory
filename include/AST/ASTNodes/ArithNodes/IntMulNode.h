//
// Created by lepoidev on 10/28/18.
//

#ifndef GAZPREABASE_INTMULNODE_H
#define GAZPREABASE_INTMULNODE_H
#include "AST/ASTNodes/BaseNodes/InfixNode.h"

class IntMulNode : public InfixNode {
public:
    IntMulNode(ASTNode *left, ASTNode *right);

};
#endif //GAZPREABASE_INTMULNODE_H
