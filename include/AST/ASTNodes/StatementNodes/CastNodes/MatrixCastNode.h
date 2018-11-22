//
// Created by andyli on 21/11/18.
//

#ifndef GAZPREABASE_MATRIXCASTNODE_H
#define GAZPREABASE_MATRIXCASTNODE_H

#include "CastExprNode.h"

class MatrixCastNode : public  CastExprNode {
public:
    MatrixCastNode(ASTNode *expr, ASTNode *matrix, int line);

    ASTNode *getMatrix();

protected:
    ASTNode *matrix = nullptr;

};


#endif //GAZPREABASE_MATRIXCASTNODE_H
