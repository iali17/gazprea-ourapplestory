//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_ANDNODE_H
#define GAZPREABASE_ANDNODE_H
#include "AST/ASTNodes/BaseNodes/BitArithNode.h"

class AndNode : public BitArithNode {
public:
    AndNode(ASTNode *left, ASTNode *right);
};
#endif //GAZPREABASE_ANDNODE_H
