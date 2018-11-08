//
// Created by kyle on 22/10/18.
//

#ifndef GAZPREABASE_RETURNNODE_H
#define GAZPREABASE_RETURNNODE_H
#include "AST/ASTNodes/BaseNodes/UnaryNode.h"

class ReturnNode : public UnaryNode {
public:
    explicit ReturnNode(ASTNode *expr, int line);

};
#endif //GAZPREABASE_RETURNNODE_H
