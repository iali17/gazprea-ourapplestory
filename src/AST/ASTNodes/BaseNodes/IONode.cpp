//
// Created by kyle on 27/10/18.
//
#include "AST/ASTNodes/BaseNodes/IONode.h"

IONode::IONode(const std::string &streamName, int line) : ASTNode(line), streamName(streamName) {}

const std::string &IONode::getStreamName() const {
    return streamName;
}
