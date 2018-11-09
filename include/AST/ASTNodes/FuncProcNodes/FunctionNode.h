//
// Created by kyle on 08/11/18.
//

#ifndef GAZPREABASE_FUNCTIONNODE_H
#define GAZPREABASE_FUNCTIONNODE_H

#include "AST/ASTNodes/BaseNodes/BaseBlockParent.h"

class FunctionNode : public BaseBlockParent {
public:
    FunctionNode(int line, std::vector<ASTNode *> *paramNodes, const std::string &retType,
                 const std::string &functionName);

    std::vector<ASTNode *> *getParamNodes() const;
    const std::string &getRetType() const;
    const std::string &getFunctionName() const;

protected:
    std::vector<ASTNode *> *paramNodes;
    std::string retType;
    std::string functionName;
};
#endif //GAZPREABASE_FUNCTIONNODE_H
