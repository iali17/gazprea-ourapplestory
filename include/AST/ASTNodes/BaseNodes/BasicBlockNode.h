//
// Created by kyle on 22/10/18.
//

#ifndef GAZPREABASE_BASICBLOCKNODE_H
#define GAZPREABASE_BASICBLOCKNODE_H
#include "ASTNode.h"
#include <vector>

class BasicBlockNode : public ASTNode {
public:
    std::vector<ASTNode *> *nodes;

    explicit BasicBlockNode(int line);

    explicit BasicBlockNode(std::vector<ASTNode *> *nodes, int line);
};

#endif //GAZPREABASE_BASICBLOCKNODE_H
