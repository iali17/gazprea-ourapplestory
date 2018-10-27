//
// Created by kyle on 27/10/18.
//

#ifndef GAZPREABASE_BASEBLOCKPARENT_H
#define GAZPREABASE_BASEBLOCKPARENT_H
#include "BlockNode.h"

class BaseBlockParent : public ASTNode {
protected:
    BlockNode * blockNode;
public:
    BaseBlockParent();
    BaseBlockParent(BlockNode *blockNode);
    BasicBlockNode * getBlock();
};
#endif //GAZPREABASE_BASEBLOCKPARENT_H
