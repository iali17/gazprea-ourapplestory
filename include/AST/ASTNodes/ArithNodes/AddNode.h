//
// Created by lepoidev on 10/28/18.
//

#ifndef GAZPREABASE_INTADDNODE_H
#define GAZPREABASE_INTADDNODE_H

#include "AST/ASTNodes/BaseNodes/InfixNode.h"

class AddNode : public InfixNode {
public:
    AddNode(ASTNode *left, ASTNode *right);
};
#endif //GAZPREABASE_INTADDNODE_H
