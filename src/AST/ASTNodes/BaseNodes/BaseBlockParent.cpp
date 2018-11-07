//
// Created by kyle on 27/10/18.
//

#include <AST/ASTNodes/BaseNodes/BaseBlockParent.h>

BaseBlockParent::BaseBlockParent(BlockNode *blockNode) : blockNode(blockNode) {}

BasicBlockNode *BaseBlockParent::getBlock() {
    return blockNode->getFullBlock();
}

BaseBlockParent::BaseBlockParent() {
    blockNode = nullptr;
}
