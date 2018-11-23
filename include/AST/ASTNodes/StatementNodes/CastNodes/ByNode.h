//
// Created by csun on 11/22/18.
//

#ifndef GAZPREABASE_BYNODE_H
#define GAZPREABASE_BYNODE_H


#include "../../BaseNodes/InfixNode.h"

class ByNode  : public InfixNode {
public:
    ByNode(ASTNode *left, ASTNode *right, int line);
};


#endif //GAZPREABASE_BYNODE_H
