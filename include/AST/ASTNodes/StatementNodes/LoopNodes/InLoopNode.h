//
// Created by kyle on 26/10/18.
//

#ifndef GAZPREABASE_INLOOPNODE_H
#define GAZPREABASE_INLOOPNODE_H
#include <AST/ASTNodes/BaseNodes/BaseLoopNode.h>

class InLoopNode : public BaseLoopNode {
public:
    InLoopNode(ASTNode *body, ASTNode *control, int line);
};
#endif //GAZPREABASE_INLOOPNODE_H
