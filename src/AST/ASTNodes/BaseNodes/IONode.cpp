//
// Created by kyle on 27/10/18.
//
#include "AST/ASTNodes/BaseNodes/IONode.h"

IONode::IONode(const std::string &streamName) : streamName(streamName) {}

const std::string &IONode::getStreamName() const {
    return streamName;
}
