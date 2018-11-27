//
// Created by kyle on 26/11/18.
//

#ifndef GAZPREABASE_SLICEASSIGN_H
#define GAZPREABASE_SLICEASSIGN_H

#include "AST/ASTNodes/BaseNodes/InfixNode.h"

class SliceAssignNode : public InfixNode {
public:
    SliceAssignNode(ASTNode *left, ASTNode *right, int line);
};

#endif //GAZPREABASE_SLICEASSIGN_H
