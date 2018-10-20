//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_BLOCKNODE_H
#define GAZPREABASE_BLOCKNODE_H
#include "ASTNode.h"
#include <vector>

class BlockNode : public ASTNode {
public:
    std::vector<ASTNode *> *nodes;
    BlockNode();

    BlockNode(std::vector<ASTNode *> *nodes);
};

#endif //GAZPREABASE_BLOCKNODE_H
