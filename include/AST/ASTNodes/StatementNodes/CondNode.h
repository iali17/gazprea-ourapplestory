//
// Created by kyle on 26/10/18.
//

#ifndef GAZPREABASE_CONDNODE_H
#define GAZPREABASE_CONDNODE_H
#include "AST/ASTNodes/BaseNodes/ASTNode.h"
#include <vector>

class CondNode : public ASTNode {
public:
    CondNode(std::vector<ASTNode *> *conds, std::vector<ASTNode *> *blocks);
    std::vector<ASTNode *> *getConds() const;
    std::vector<ASTNode *> *getBlocks() const;
    bool isHasElse() const;

protected:
    std::vector<ASTNode *> *conds;
    std::vector<ASTNode *> *blocks;
    bool hasElse;
};

#endif //GAZPREABASE_CONDNODE_H
