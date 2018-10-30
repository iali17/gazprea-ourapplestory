//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_NEQNODE_H
#define GAZPREABASE_NEQNODE_H
#include "AST/ASTNodes/BaseNodes/CompNode.h"

class NEQNode : public CompNode {
public:
    NEQNode(ASTNode *left, ASTNode *right);
};
#endif //GAZPREABASE_NEQNODE_H
