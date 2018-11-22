//
// Created by andyli on 21/11/18.
//

#ifndef GAZPREABASE_VECTORCASTNODE_H
#define GAZPREABASE_VECTORCASTNODE_H

#include "CastExprNode.h"

class VectorCastNode : public  CastExprNode {
public:
    VectorCastNode(ASTNode *expr, ASTNode *vector, int line);

    ASTNode *getVector();

protected:
    ASTNode *vector = nullptr;

};

#endif //GAZPREABASE_VECTORCASTNODE_H
