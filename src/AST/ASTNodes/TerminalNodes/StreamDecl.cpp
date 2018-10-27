//
// Created by kyle on 27/10/18.
//

#include "AST/ASTNodes/TerminalNodes/StreamDecl.h"

StreamDeclNode::StreamDeclNode(const std::string &id, int streamType) : id(id), streamType(streamType) {
    type = streamType;
}

const std::string &StreamDeclNode::getId() const {
    return id;
}

int StreamDeclNode::getStreamType() const {
    return streamType;
}
