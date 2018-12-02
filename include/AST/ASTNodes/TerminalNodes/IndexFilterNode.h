//
// Created by lepoidev on 12/2/18.
//

#ifndef GAZPREABASE_INDEXFILTERNODE_H
#define GAZPREABASE_INDEXFILTERNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class IndexFilterNode : public ASTNode{
protected:
    ASTNode * filterNode;
    int index;
public:
    ASTNode *getFilterNode() const;
    int getIndex() const;
    IndexFilterNode(int, ASTNode *filterNode, int index);
};

#endif //GAZPREABASE_INDEXFILTERNODE_H
