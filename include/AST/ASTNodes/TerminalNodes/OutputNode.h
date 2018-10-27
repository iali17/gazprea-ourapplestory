//
// Created by kyle on 27/10/18.
//

#ifndef GAZPREABASE_OUTPUTNODE_H
#define GAZPREABASE_OUTPUTNODE_H
#include "AST/ASTNodes/BaseNodes/IONode.h"

class OutputNode : public IONode {
public:
    OutputNode(const std::string &streamName, ASTNode *expr);
    ASTNode *getExpr() const;

protected:
    ASTNode * expr;
};
#endif //GAZPREABASE_OUTPUTNODE_H
