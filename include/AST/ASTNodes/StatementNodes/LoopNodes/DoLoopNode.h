//
// Created by kyle on 26/10/18.
//

#ifndef GAZPREABASE_DOLOOPNODE_H
#define GAZPREABASE_DOLOOPNODE_H
#include "../../BaseNodes/BaseLoopNode.h"

class DoLoopNode : public BaseLoopNode {
public:
    DoLoopNode(ASTNode *body, ASTNode *control);
};
#endif //GAZPREABASE_DOLOOPNODE_H
