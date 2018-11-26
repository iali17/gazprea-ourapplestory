//
// Created by kyle on 25/11/18.
//

#ifndef GAZPREABASE_CONCATENATIONNODE_H
#define GAZPREABASE_CONCATENATIONNODE_H

#include "AST/ASTNodes/BaseNodes/InfixNode.h"

class ConcatenationNode : public InfixNode {
public:
    ConcatenationNode(ASTNode *left, ASTNode *right, int line);
};

#endif //GAZPREABASE_CONCATENATIONNODE_H
