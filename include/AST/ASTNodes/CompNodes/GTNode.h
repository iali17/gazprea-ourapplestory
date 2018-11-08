//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_GTNODE_H
#define GAZPREABASE_GTNODE_H
#include "AST/ASTNodes/BaseNodes/CompNode.h"

class GTNode : public CompNode {
public:
    GTNode(ASTNode *left, ASTNode *right, int line);
};
#endif //GAZPREABASE_GTNODE_H
