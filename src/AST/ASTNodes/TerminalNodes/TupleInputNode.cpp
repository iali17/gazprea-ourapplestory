//
// Created by kyle on 08/11/18.
//

#include "AST/AST.h"

TupleInputNode::TupleInputNode(const std::string &streamName, ASTNode *indexNode) : IONode(streamName) {
    indexTupleNode = dynamic_cast<IndexTupleNode *>(indexNode);
}

IndexTupleNode *TupleInputNode::getIndexTupleNode() const {
    return indexTupleNode;
}
