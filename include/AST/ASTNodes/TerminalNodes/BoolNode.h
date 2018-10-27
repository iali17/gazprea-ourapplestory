//
// Created by lepoidev on 10/26/18.
//

#ifndef GAZPREABASE_BOOLNODE_H
#define GAZPREABASE_BOOLNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class BoolNode : public ASTNode {
public:
    bool getVal() const;
    BoolNode(bool val);

protected:
    bool val;
};
#endif //GAZPREABASE_BOOLNODE_H