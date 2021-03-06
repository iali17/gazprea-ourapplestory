//
// Created by kyle on 27/10/18.
//

#ifndef GAZPREABASE_IONODE_H
#define GAZPREABASE_IONODE_H

#include "ASTNode.h"

class IONode : public ASTNode {
public:
    explicit IONode(const std::string &streamName, int line);
    const std::string &getStreamName() const;

protected:
    std::string streamName;
};

#endif //GAZPREABASE_IONODE_H
