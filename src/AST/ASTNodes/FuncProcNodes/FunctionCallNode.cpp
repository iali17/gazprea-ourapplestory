//
// Created by lepoidev on 11/9/18.
//

#include "AST/AST.h"

FunctionCallNode::FunctionCallNode(int lines , const std::string &functionName, std::vector<ASTNode *> *arguments) : ASTNode(lines),

functionName (functionName), arguments(arguments) {}

const std::string &FunctionCallNode::getFunctionName() const {
    return functionName;
}

std::vector<ASTNode *> *FunctionCallNode::getArguments() {
    return arguments;
}
