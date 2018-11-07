//
// Created by lepoidev on 10/26/18.
//

#ifndef GAZPREABASE_REALNODE_H
#define GAZPREABASE_REALNODE_H

#include "AST/ASTNodes/BaseNodes/UnaryNode.h"

class RealNode : public ASTNode {
public:
    explicit RealNode(float val);
    float getVal() const;

protected:
    float val;
};

#endif //GAZPREABASE_REALNODE_H
