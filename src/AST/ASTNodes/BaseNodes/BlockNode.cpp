//
// Created by kyle on 20/10/18.
//

#include <AST/ASTNodes/BaseNodes/BlockNode.h>

BlockNode::BlockNode(ASTNode *declBlock, ASTNode *stateBlock) : declBlock(declBlock),
                                                                              stateBlock(stateBlock) {}

ASTNode *BlockNode::getDeclBlock() const {
    return declBlock;
}

ASTNode *BlockNode::getStateBlock() const {
    return stateBlock;
}
