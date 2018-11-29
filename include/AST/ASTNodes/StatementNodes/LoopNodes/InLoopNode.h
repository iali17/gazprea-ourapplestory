//
// Created by kyle on 26/10/18.
//

#ifndef GAZPREABASE_INLOOPNODE_H
#define GAZPREABASE_INLOOPNODE_H
#include <AST/ASTNodes/BaseNodes/BaseLoopNode.h>

class InLoopNode : public BaseLoopNode {
protected:
    ASTNode   * domain;
    std::string loopVar;
public:
    InLoopNode(BlockNode *blockNode, ASTNode *control, int line, ASTNode *domain, const std::string &loopVar);
    ASTNode *getDomain() const;
    const std::string &getLoopVar() const;
};
#endif //GAZPREABASE_INLOOPNODE_H
