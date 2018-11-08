//
// Created by kyle on 05/11/18.
//

#include "AST/AST.h"

ASTNode *IndexTupleNode::getIndex() const {
    return index;
}

IDNode *IndexTupleNode::getIdNode() const {
    return idNode;
}

IndexTupleNode::IndexTupleNode(ASTNode *index, IDNode *idNode, int line) : ASTNode(line), index(index), idNode(idNode) {}
