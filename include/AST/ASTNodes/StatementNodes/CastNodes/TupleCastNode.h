//
// Created by andyli on 21/11/18.
//

#ifndef GAZPREABASE_TUPLECASTNODE_H
#define GAZPREABASE_TUPLECASTNODE_H

#include "CastExprNode.h"

class TupleCastNode : public  CastExprNode {
public:
    TupleCastNode(ASTNode *expr, ASTNode *tuple, int line);

    ASTNode *getTuple();

protected:
    ASTNode *tuple = nullptr;

};

#endif //GAZPREABASE_TUPLECASTNODE_H
