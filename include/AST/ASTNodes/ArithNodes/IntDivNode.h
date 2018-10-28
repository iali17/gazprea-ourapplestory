//
// Created by lepoidev on 10/28/18.
//

#ifndef GAZPREABASE_INTDIVNODE_H
#define GAZPREABASE_INTDIVNODE_H
#include "AST/ASTNodes/BaseNodes/InfixNode.h"

class IntDivNode : public InfixNode {
public:
    IntDivNode(ASTNode *left, ASTNode *right);
};
#endif //GAZPREABASE_INTDIVNODE_H
