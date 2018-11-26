//
// Created by lepoidev on 11/26/18.
//

#ifndef GAZPREABASE_FILTERNODE_H
#define GAZPREABASE_FILTERNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class FilterNode : public ASTNode {
protected:
    std::string             loopVar;
    std::vector<ASTNode *> *condNodes;
    ASTNode                *range;
    ASTNode                *notCondNodes;
public:
    const std::string &getLoopVar() const;
    std::vector<ASTNode *> *getCondNodes() const;
    ASTNode *getRange() const;
    ASTNode *getNotCondNodes() const;
    FilterNode(int, const std::string &loopVar, std::vector<ASTNode *> *condNodes, ASTNode *range, ASTNode * notCondNodes);
};

#endif //GAZPREABASE_FILTERNODE_H
