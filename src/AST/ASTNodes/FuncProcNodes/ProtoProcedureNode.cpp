//
// Created by iali1 on 11/4/18.
//

#include <string>
#include <AST/ASTNodes/FuncProcNodes/ProtoProcedureNode.h>

ProtoProcedureNode::ProtoProcedureNode(std::vector<ASTNode *> *paramNodes, const std::string &retType,
                                       const std::string &procedureName) : paramNodes(paramNodes), retType(retType),
                                                                           procedureName(procedureName) {}

std::vector<ASTNode *> *ProtoProcedureNode::getParamNodes() const {
    return paramNodes;
}

const std::string &ProtoProcedureNode::getRetType() const {
    return retType;
}

const std::string &ProtoProcedureNode::getProcedureName() const {
    return procedureName;
}
