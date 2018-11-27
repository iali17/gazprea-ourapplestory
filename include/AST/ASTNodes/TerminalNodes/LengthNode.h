//
// Created by kyle on 26/11/18.
//

#ifndef GAZPREABASE_LENGTHNODE_H
#define GAZPREABASE_LENGTHNODE_H

#include "AST/ASTNodes/BaseNodes/UnaryNode.h"

class LengthNode : public UnaryNode {
public:
    LengthNode(ASTNode *expr, int line);
};

#endif //GAZPREABASE_LENGTHNODE_H
