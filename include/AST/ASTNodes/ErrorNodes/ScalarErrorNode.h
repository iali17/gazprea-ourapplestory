//
// Created by ali5 on 11/5/18.
//

#ifndef GAZPREABASE_SCALARNODE_H
#define GAZPREABASE_SCALARNODE_H

#include <string>
#include "ErrorNode.h"

// This class deals with errors involving scalar to scalar type conversions
class ScalarErrorNode : public ErrorNode {
public:
    ScalarErrorNode(std::string &left, std::string &right, int line);

    std::string getNode() override;

protected:
    std::string left, right;
    int line;
};

#endif //GAZPREABASE_SCALARNODE_H
