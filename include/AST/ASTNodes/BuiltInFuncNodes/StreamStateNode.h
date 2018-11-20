//
// Created by iali1 on 11/20/18.
//

#ifndef GAZPREABASE_STREAMSTATENODE_H
#define GAZPREABASE_STREAMSTATENODE_H

#include "AST/ASTNodes/BaseNodes/BuiltIn.h"

class StreamStateNode : public BuiltIn {
public:
    StreamStateNode(int line, const std::string &streamName);
};


#endif //GAZPREABASE_STREAMSTATENODE_H
