//
// Created by ali5 on 10/30/18.
//

#ifndef GAZPREABASE_CASTEXPRNODE_H
#define GAZPREABASE_CASTEXPRNODE_H
#include "../../BaseNodes/UnaryNode.h"

class CastExprNode : public UnaryNode {
public:
    enum castType {SCALAR_CAST, TUPLE_CAST, VECTOR_CAST, MATRIX_CAST};

    CastExprNode(ASTNode *expr, castType cType,int line);
    castType getCastType() const;

private:
    castType cType;
};

#endif //GAZPREABASE_CASTEXPRNODE_H
