//
// Created by ali5 on 11/18/18.
//

#ifndef GAZPREABASE_INDEXNODE_H
#define GAZPREABASE_INDEXNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class IndexNode : public ASTNode {
public:
    IndexNode(ASTNode *LHS, std::vector<ASTNode *> *indexExpr, int line);
    ASTNode *getLHS() const;
    std::vector<ASTNode *> *getIndexExpr() const;

protected:
    ASTNode *LHS;
    std::vector<ASTNode *> *indexExpr;
};

#endif //GAZPREABASE_INDEXNODE_H
