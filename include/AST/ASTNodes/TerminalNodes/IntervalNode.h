//
// Created by ali5 on 11/18/18.
//

#ifndef GAZPREABASE_INTERVALNODE_H
#define GAZPREABASE_INTERVALNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class IntervalNode : public ASTNode {
public:
    IntervalNode(ASTNode* leftBound, ASTNode* rightBound, int line);
    ASTNode* getLeftBound();
    ASTNode* getRightBound();

protected:
    ASTNode* leftBound;
    ASTNode* rightBound;
};

#endif //GAZPREABASE_INTERVALNODE_H
