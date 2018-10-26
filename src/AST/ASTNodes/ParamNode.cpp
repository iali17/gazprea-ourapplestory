//
// Created by kyle on 22/10/18.
//

#include "AST/ASTNodes/ParamNode.h"

ParamNode::ParamNode(const std::string &declaredType, const std::string &varName) : declaredType(declaredType),
                                                                                    varName(varName) {}

const std::string &ParamNode::getDeclaredType() const {
    return declaredType;
}

const std::string &ParamNode::getVarName() const {
    return varName;
}
