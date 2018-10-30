//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_XORNODE_H
#define GAZPREABASE_XORNODE_H
#include "AST/ASTNodes/BaseNodes/BitArithNode.h"

class XOrNode : public BitArithNode {
public:
    XOrNode(ASTNode *left, ASTNode *right);
};
#endif //GAZPREABASE_XORNODE_H
