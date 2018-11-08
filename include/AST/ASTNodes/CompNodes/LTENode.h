//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_LTENODE_H
#define GAZPREABASE_LTENODE_H
#include "AST/ASTNodes/BaseNodes/CompNode.h"

class LTENode : public CompNode {
public:
    LTENode(ASTNode *left, ASTNode *right, int line);
};
#endif //GAZPREABASE_LTENODE_H
