//
// Created by lepoidev on 10/28/18.
//

#ifndef GAZPREABASE_INTADDNODE_H
#define GAZPREABASE_INTADDNODE_H

#include "AST/ASTNodes/BaseNodes/InfixNode.h"

class IntAddNode : public InfixNode {
public:
    IntAddNode(ASTNode *left, ASTNode *right);
};
#endif //GAZPREABASE_INTADDNODE_H
