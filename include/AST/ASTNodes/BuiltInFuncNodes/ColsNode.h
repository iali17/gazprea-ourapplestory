//
// Created by lepoidev on 12/3/18.
//

#ifndef GAZPREABASE_COLSNODE_H
#define GAZPREABASE_COLSNODE_H

#include "AST/ASTNodes/BaseNodes/UnaryNode.h"

class ColsNode : public UnaryNode {
public:
    ColsNode(ASTNode *expr, int line);
};

#endif //GAZPREABASE_COLSNODE_H
