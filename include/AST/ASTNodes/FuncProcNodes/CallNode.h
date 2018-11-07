//
// Created by iali1 on 10/28/18.
//
#ifndef GAZPREABASE_CALLNODE_H
#define GAZPREABASE_CALLNODE_H
#include <string>
#include <vector>
#include <AST/ASTNodes/BaseNodes/ASTNode.h>

class CallNode : public ASTNode {
public:
    CallNode(std::vector<ASTNode *> *exprNodes, const std::string &procedureName);
    std::vector<ASTNode *> *getExprNodes() const;
    const std::string &getProcedureName() const;

protected:
    std::vector<ASTNode *> *exprNodes;
    std::string procedureName;
};
#endif //GAZPREABASE_CALLNODE_H