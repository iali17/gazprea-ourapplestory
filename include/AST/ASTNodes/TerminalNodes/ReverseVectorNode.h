//
// Created by kyle on 22/11/18.
//

#ifndef GAZPREABASE_REVERSEVECTORNODE_H
#define GAZPREABASE_REVERSEVECTORNODE_H

#include "../../../AST/ASTNodes/BaseNodes/UnaryNode.h"

class ReverseVectorNode : public UnaryNode{
public:
    ReverseVectorNode(ASTNode *expr, int line);
};
#endif //GAZPREABASE_REVERSEVECTORNODE_H
