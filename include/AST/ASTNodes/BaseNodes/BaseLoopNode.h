//
// Created by kyle on 26/10/18.
//

#ifndef GAZPREABASE_BASELOOPNODE_H
#define GAZPREABASE_BASELOOPNODE_H
#include "AST/ASTNodes/BaseNodes/ASTNode.h"

/**
 * intended to represent a loop with 2 main components
 *  - control (expression, iterator, or none)
 *  - block
 */
class BaseLoopNode : public ASTNode {
public:
    BaseLoopNode(ASTNode *body);
    BaseLoopNode(ASTNode *body, ASTNode *control);
    ASTNode *getBody() const;
    ASTNode *getControl() const;

protected:
    ASTNode * body;
    ASTNode * control;
};
#endif //GAZPREABASE_BASELOOPNODE_H
