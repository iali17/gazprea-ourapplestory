//
// Created by kyle on 08/11/18.
//

#include "AST/AST.h"

FunctionNode::FunctionNode(int line, std::vector<ASTNode *> *paramNodes, const std::string &retType,
                           const std::string &functionName) : BaseBlockParent(line), paramNodes(paramNodes),
                                                              retType(retType), functionName(functionName) {}

std::vector<ASTNode *> *FunctionNode::getParamNodes() const {
    return paramNodes;
}

const std::string &FunctionNode::getRetType() const {
    return retType;
}

const std::string &FunctionNode::getFunctionName() const {
    return functionName;
}
