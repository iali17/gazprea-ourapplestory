//
// Created by lepoidev on 11/2/18.
//

#ifndef GAZPREABASE_BREAKNODE_H
#define GAZPREABASE_BREAKNODE_H
#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class BreakNode : public ASTNode {
public:
    explicit BreakNode(int line);
};
#endif //GAZPREABASE_BREAKNODE_H
