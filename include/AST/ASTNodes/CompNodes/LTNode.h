//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_LTNODE_H
#define GAZPREABASE_LTNODE_H
#include "AST/ASTNodes/BaseNodes/CompNode.h"

class LTNode : public CompNode {
public:
    LTNode(ASTNode *left, ASTNode *right, int line);

    int getLine();

protected:
    int line;
};
#endif //GAZPREABASE_LTNODE_H
