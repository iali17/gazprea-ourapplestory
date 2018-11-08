//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_GEQNODE_H
#define GAZPREABASE_GEQNODE_H
#include "AST/ASTNodes/BaseNodes/CompNode.h"

class GTENode : public CompNode {
public:
    GTENode(ASTNode *left, ASTNode *right, int line);
};
#endif //GAZPREABASE_GEQNODE_H
