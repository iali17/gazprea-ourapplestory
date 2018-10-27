//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_BLOCKNODE_H
#define GAZPREABASE_BLOCKNODE_H
#include "ASTNode.h"
#include "BasicBlockNode.h"
#include <vector>

class BlockNode : public ASTNode {
public:
    BlockNode(ASTNode *declBlock, ASTNode *stateBlock);
    ASTNode *getDeclBlock() const;
    ASTNode *getStateBlock() const;
    BasicBlockNode *getFullBlock();

protected:
    ASTNode *declBlock;
    ASTNode *stateBlock;
};


#endif //GAZPREABASE_BLOCKNODE_H
