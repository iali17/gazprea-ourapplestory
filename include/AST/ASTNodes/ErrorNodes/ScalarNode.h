//
// Created by ali5 on 11/5/18.
//

#ifndef GAZPREABASE_SCALARNODE_H
#define GAZPREABASE_SCALARNODE_H

#include "ErrorNode.h"
#include "../../../../../../../../../usr/include/c++/5/string"

// This class deals with errors involving scalar to scalar type conversions
class ScalarNode : public ErrorNode {
public:
    ScalarNode(std::string left, std::string right, int line);

    std::string getNode() override;

protected:
    std::string left, right;
    int line;
};

#endif //GAZPREABASE_SCALARNODE_H
