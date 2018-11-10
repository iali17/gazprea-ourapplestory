//
// Created by lepoidev on 11/9/18.
//

#ifndef GAZPREABASE_FUNCTIONCALLNODE_H
#define GAZPREABASE_FUNCTIONCALLNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class FunctionCallNode : public ASTNode {
protected:
    std::string functionName;
    std::vector<ASTNode *> * arguments;
public:
    const std::string &getFunctionName() const;
    std::vector<ASTNode *> *getArguments();
    FunctionCallNode(int lines, const std::string &functionName, std::vector<ASTNode *> *arguments);
};
#endif //GAZPREABASE_FUNCTIONCALLNODE_H
