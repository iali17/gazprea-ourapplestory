//
// Created by kyle on 27/10/18.
//

#ifndef GAZPREABASE_STREAMDECL_H
#define GAZPREABASE_STREAMDECL_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class StreamDeclNode : public ASTNode {
public:
    StreamDeclNode(const std::string &id, int streamType, int line);
    const std::string &getId() const;
    int getStreamType() const;

protected:
    std::string id;
    int streamType;
};

#endif //GAZPREABASE_STREAMDECL_H
