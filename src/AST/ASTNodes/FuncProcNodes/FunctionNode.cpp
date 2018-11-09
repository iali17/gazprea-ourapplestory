//
// Created by kyle on 08/11/18.
//

#include "AST/AST.h"

FunctionNode::FunctionNode(BlockNode *blockNode, std::vector<ASTNode *> *paramNodes, const std::string &retType,
                             const std::string &functionName, int line, TupleType * tupleType)
                             : BaseBlockParent(blockNode, line), paramNodes(paramNodes),
                             retType(retType), functionName(functionName), tupleType(tupleType){}

std::vector<ASTNode *> *FunctionNode::getParamNodes() const {
    return paramNodes;
}

const std::string &FunctionNode::getRetType() const {
    return retType;
}

const std::string &FunctionNode::getFunctionName() const {
    return functionName;
}

TupleType *FunctionNode::getTupleType() const {
    return tupleType;
}
