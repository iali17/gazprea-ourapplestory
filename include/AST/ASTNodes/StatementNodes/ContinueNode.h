//
// Created by lepoidev on 11/2/18.
//

#ifndef GAZPREABASE_CONTINUENODE_H
#define GAZPREABASE_CONTINUENODE_H
#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class ContinueNode : public ASTNode {
public:
    explicit ContinueNode(int line);
};
#endif //GAZPREABASE_CONTINUENODE_H
