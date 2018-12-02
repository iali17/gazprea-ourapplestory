//
// Created by lepoidev on 12/2/18.
//

#include "AST/AST.h"

IndexFilterNode::IndexFilterNode(int line, ASTNode *filterNode, int index) : ASTNode(line),

filterNode (filterNode), index(index) {}

ASTNode *IndexFilterNode::getFilterNode() const {
    return filterNode;
}

int IndexFilterNode::getIndex() const {
    return index;
}
