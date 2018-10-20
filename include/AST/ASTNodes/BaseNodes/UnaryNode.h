//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_UNARYNODES_H
#define GAZPREABASE_UNARYNODES_H

#include "ASTNode.h"

class UnaryNode : public ASTNode {
public:
    UnaryNode(ASTNode *expr);

    ASTNode *getExpr() const;

protected:
    ASTNode *expr;
};

#endif //GAZPREABASE_UNARYNODES_H
