//
// Created by iali1 on 11/4/18.
//
#include <string>
#include <AST/ASTNodes/FuncProcNodes/ProtoProcedureNode.h>

ProtoProcedureNode::ProtoProcedureNode(std::vector<ASTNode *> *paramNodes, const std::string &retType,
                                       const std::string &procedureName, int line, TupleTypeNode *tupleType, int gType)
                                       : ASTNode(line), paramNodes(paramNodes), retType(retType),
                                       procedureName(procedureName), tupleType(tupleType), gType(gType) {}

std::vector<ASTNode *> *ProtoProcedureNode::getParamNodes() const {
    return paramNodes;
}

const std::string &ProtoProcedureNode::getRetType() const {
    return retType;
}

const std::string &ProtoProcedureNode::getProcedureName() const {
    return procedureName;
}

TupleTypeNode *ProtoProcedureNode::getTupleType() const {
    return tupleType;
}

int ProtoProcedureNode::getGType() const {
    return gType;
}
