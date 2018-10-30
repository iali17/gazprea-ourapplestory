//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_NEGATENODE_H
#define GAZPREABASE_NEGATENODE_H
#include "AST/ASTNodes/BaseNodes/UnaryNode.h"

class NegateNode : public UnaryNode {
public:
    NegateNode(ASTNode *expr);
};
#endif //GAZPREABASE_NEGATENODE_H
