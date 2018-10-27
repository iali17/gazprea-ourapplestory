//
// Created by kyle on 26/10/18.
//

#ifndef GAZPREABASE_LOOPNODE_H
#define GAZPREABASE_LOOPNODE_H
#include "../../BaseNodes/BaseLoopNode.h"

class LoopNode : public BaseLoopNode {
public:
    LoopNode(ASTNode *body);
    LoopNode(ASTNode *body, ASTNode *control);
};
#endif //GAZPREABASE_LOOPNODE_H
