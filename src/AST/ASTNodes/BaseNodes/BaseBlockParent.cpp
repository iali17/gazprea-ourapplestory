//
// Created by kyle on 27/10/18.
//

#include <AST/ASTNodes/BaseNodes/BaseBlockParent.h>

BaseBlockParent::BaseBlockParent(BlockNode *blockNode, int line) : ASTNode(line), blockNode(blockNode) {}

BasicBlockNode *BaseBlockParent::getBlock() {
    return blockNode->getFullBlock();
}

BaseBlockParent::BaseBlockParent(int line) : ASTNode(line) {
    blockNode = nullptr;
}
