//
// Created by lepoidev on 11/26/18.
//

#include "AST/AST.h"

FilterNode::FilterNode(int line, const std::string &loopVar, std::vector<ASTNode *> *condNodes, ASTNode *range, ASTNode *notCondNodes) : ASTNode(line), loopVar (loopVar), condNodes(condNodes), range(range), notCondNodes(notCondNodes) {}

const std::string &FilterNode::getLoopVar() const {
    return loopVar;
}

std::vector<ASTNode *> *FilterNode::getCondNodes() const {
    return condNodes;
}

ASTNode *FilterNode::getRange() const {
    return range;
}

ASTNode *FilterNode::getNotCondNodes() const {
    return notCondNodes;
}
