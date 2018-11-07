//
// Created by kyle on 22/10/18.
//
#include "AST/ASTNodes/FuncProcNodes/ParamNode.h"

const std::string &ParamNode::getDeclaredType() const {
    return declaredType;
}

const std::string &ParamNode::getVarName() const {
    return varName;
}

bool ParamNode::isIsVar() const {
    return isVar;
}

ParamNode::ParamNode(const std::string &declaredType, const std::string &varName, bool isVar) : declaredType(
        declaredType), varName(varName), isVar(isVar) {}
