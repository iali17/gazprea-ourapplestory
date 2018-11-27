//
// Created by ali5 on 11/18/18.
//

#ifndef GAZPREABASE_INDEXNODE_H
#define GAZPREABASE_INDEXNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class IndexNode : public ASTNode {
public:
    IndexNode(ASTNode *LHS, std::vector<ASTNode *> *indexExpr, int line, bool slice = false);
    ASTNode *getLHS() const;
    std::vector<ASTNode *> *getIndexExpr() const;
    bool isSlice() const;
protected:
    ASTNode *LHS;
    std::vector<ASTNode *> *indexExpr;
    bool slice;
};

#endif //GAZPREABASE_INDEXNODE_H
