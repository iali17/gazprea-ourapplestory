//
// Created by lepoidev on 10/28/18.
//

#ifndef GAZPREABASE_INTSUBNODE_H
#define GAZPREABASE_INTSUBNODE_H
#include "AST/ASTNodes/BaseNodes/InfixNode.h"
class SubNode : public InfixNode {
public:
    SubNode(ASTNode *left, ASTNode *right);
};

#endif //GAZPREABASE_INTSUBNODE_H
