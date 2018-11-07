//
// Created by kyle on 29/10/18.
//

#ifndef GAZPREABASE_ORNODE_H
#define GAZPREABASE_ORNODE_H
#include "AST/ASTNodes/BaseNodes/BitArithNode.h"

class OrNode : public BitArithNode {
public:
    OrNode(ASTNode *left, ASTNode *right, int line);

    int getLine();

protected:
    int line;
};
#endif //GAZPREABASE_ORNODE_H
