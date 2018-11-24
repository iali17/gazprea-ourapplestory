//
// Created by lepoidev on 11/23/18.
//

#ifndef GAZPREABASE_GENERATORNODE_H
#define GAZPREABASE_GENERATORNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class GeneratorNode : public ASTNode {
protected:
    std::vector<std::string> *loopVars;
    std::vector<ASTNode *>   *ranges;
    ASTNode                  *exprNode;
public:
    std::vector<std::string> *getLoopVars() const;
    GeneratorNode(int line, std::vector<std::string> *loopVars, std::vector<ASTNode *> *ranges, ASTNode *condNode);
    std::vector<ASTNode *> *getRanges() const;
    ASTNode *getExprNode() const;
};
#endif //GAZPREABASE_GENERATORNODE_H
