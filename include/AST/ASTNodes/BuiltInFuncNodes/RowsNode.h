//
// Created by lepoidev on 12/3/18.
//

#ifndef GAZPREABASE_ROWSNODE_H
#define GAZPREABASE_ROWSNODE_H

#include "AST/ASTNodes/BaseNodes/UnaryNode.h"

class RowsNode : public UnaryNode {
public:
    RowsNode(ASTNode *expr, int line);
};

#endif //GAZPREABASE_ROWSNODE_H
