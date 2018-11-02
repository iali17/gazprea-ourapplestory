//
// Created by ali5 on 10/30/18.
//

#ifndef GAZPREABASE_CASTEXPRNODE_H
#define GAZPREABASE_CASTEXPRNODE_H

#include "AST/ASTNodes/BaseNodes/UnaryNode.h"

class CastExprNode : public UnaryNode {
public:
    CastExprNode(ASTNode *expr, const std::string &type);

    const std::string &getTypeString() const;

protected:
    std::string type;
};

#endif //GAZPREABASE_CASTEXPRNODE_H