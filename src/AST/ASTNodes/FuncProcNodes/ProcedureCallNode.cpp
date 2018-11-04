//
// Created by iali on 11/2/18.
//
#include "AST/ASTNodes/FuncProcNodes/ProcedureCallNode.h"

const std::string &ProcedureCallNode::getVarName() const {
    return varName;
}

const std::string &ProcedureCallNode::getProcedureName() const {
    return procedureName;
}

std::vector<ParamNode *> *ProcedureCallNode::getExprNode() const {
    return exprNode;
}

std::vector<std::string> *ProcedureCallNode::getTypeIds() const {
    return typeIds;
}

bool ProcedureCallNode::isConstant() const {
    return constant;
}

ProcedureCallNode::ProcedureCallNode(const std::string &varName, const std::string &procedureName,
                                     std::vector<ParamNode *> *exprNode, std::vector<std::string> *typeIds, bool constant)
                                     : varName(varName), procedureName(procedureName), exprNode(exprNode),
                                     typeIds(typeIds), constant(constant) {}

ProcedureCallNode::ProcedureCallNode(const std::string &varName, const std::string &procedureName,
                                     std::vector<ParamNode *> *exprNode) : varName(varName), procedureName(procedureName),
                                                                         exprNode(exprNode) {}

