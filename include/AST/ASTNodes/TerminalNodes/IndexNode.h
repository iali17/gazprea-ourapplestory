//
// Created by ali5 on 11/18/18.
//

#ifndef GAZPREABASE_INDEXNODE_H
#define GAZPREABASE_INDEXNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class IndexNode : public ASTNode {
public:
    explicit IndexNode(std::vector<ASTNode *>* exprIndex, int indexSize, int line);
    std::vector<ASTNode *>* getExprs();
    int getIndexSize();

protected:
    int indexSize;
    std::vector<ASTNode *>* exprIndex;
};

#endif //GAZPREABASE_INDEXNODE_H
