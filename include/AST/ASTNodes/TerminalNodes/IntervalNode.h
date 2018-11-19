//
// Created by ali5 on 11/18/18.
//

#ifndef GAZPREABASE_INTERVALNODE_H
#define GAZPREABASE_INTERVALNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class IntervalNode : ASTNode {
public:
    IntervalNode(int leftBound, int rightBound, int line);
    int getLeftBound();
    int getRightBound();

protected:
    int leftBound, rightBound;
};

#endif //GAZPREABASE_INTERVALNODE_H
