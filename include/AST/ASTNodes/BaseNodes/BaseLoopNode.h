//
// Created by kyle on 26/10/18.
//

#ifndef GAZPREABASE_BASELOOPNODE_H
#define GAZPREABASE_BASELOOPNODE_H
#include "AST/ASTNodes/BaseNodes/BaseBlockParent.h"

/**
 * intended to represent a loop with 2 main components
 *  - control (expression, iterator, or none)
 *  - block
 */
class BaseLoopNode : public BaseBlockParent {
public:
    BaseLoopNode(ASTNode *control);

    ASTNode *getControl() const;

    BaseLoopNode(BlockNode *blockNode, ASTNode *control);

protected:
    ASTNode * control;
};
#endif //GAZPREABASE_BASELOOPNODE_H
