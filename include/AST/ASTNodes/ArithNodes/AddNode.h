//
// Created by lepoidev on 10/28/18.
//

#ifndef GAZPREABASE_INTADDNODE_H
#define GAZPREABASE_INTADDNODE_H

#include "AST/ASTNodes/BaseNodes/ArithNode.h"

class AddNode : public ArithNode {
public:
    AddNode(ASTNode *left, ASTNode *right, int line);
};
#endif //GAZPREABASE_INTADDNODE_H
