//
// Created by kyle on 20/10/18.
//

#include <AST/ASTNodes/BaseNodes/BlockNode.h>

BlockNode::BlockNode(ASTNode *declBlock, ASTNode *stateBlock, int line) : ASTNode(line), declBlock(declBlock),
                                                                              stateBlock(stateBlock) {}

ASTNode *BlockNode::getDeclBlock() const {
    return declBlock;
}

ASTNode *BlockNode::getStateBlock() const {
    return stateBlock;
}

BasicBlockNode *BlockNode::getFullBlock() {
    auto bb1 = (BasicBlockNode *) declBlock;
    auto bb2 = (BasicBlockNode *) stateBlock;//new std::vector<ASTNode *>;
    auto full = new std::vector<ASTNode *>;
    full->insert(full->end(), bb1->nodes->begin(), bb1->nodes->end());
    full->insert(full->end(), bb2->nodes->begin(), bb2->nodes->end());
    return new BasicBlockNode(full);
}
