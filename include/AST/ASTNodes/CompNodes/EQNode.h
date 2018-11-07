//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_EQNODE_H
#define GAZPREABASE_EQNODE_H
#include "AST/ASTNodes/BaseNodes/CompNode.h"

class EQNode : public CompNode {
public:
    EQNode(ASTNode *left, ASTNode *right, int line);

    int getLine();

protected:
    int line;
};
#endif //GAZPREABASE_EQNODE_H
