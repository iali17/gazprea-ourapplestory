//
// Created by kyle on 08/11/18.
//

#include "AST/AST.h"

TupleInputNode::TupleInputNode(const std::string &streamName, ASTNode *indexNode, int line) : IONode(streamName, line) {
    indexTupleNode = dynamic_cast<IndexTupleNode *>(indexNode);
}

IndexTupleNode *TupleInputNode::getIndexTupleNode() const {
    return indexTupleNode;
}
