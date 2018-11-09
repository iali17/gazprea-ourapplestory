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
std::vector<ASTNode *> *ProcedureCallNode::getExprNode() const {
    return exprNode;
}
std::vector<std::string> *ProcedureCallNode::getTypeIds() const {
    return typeIds;
}
bool ProcedureCallNode::isConstant() const {
    return constant;
}

int ProcedureCallNode::getUnOp() const {
    return unOp;
}

TupleType *ProcedureCallNode::getTupleType() const {
    return tupleType;
}

ProcedureCallNode::ProcedureCallNode(const std::string &varName, const std::string &procedureName,
                                     std::vector<ASTNode *> *exprNode, std::vector<std::string> *typeIds, bool constant,
                                     int line, int unOp, TupleType *tupleType)
                                     : ASTNode(line), varName(varName), procedureName(procedureName), unOp(unOp),
                                     exprNode(exprNode), typeIds(typeIds), constant(constant), tupleType(tupleType) {}

ProcedureCallNode::ProcedureCallNode(const std::string &varName, const std::string &procedureName,
                                     std::vector<ASTNode *> *exprNode, int line, int unOp, TupleType *tupleType)
                                     : ASTNode(line), varName(varName), procedureName(procedureName),
                                     unOp(unOp), exprNode(exprNode), tupleType(tupleType) {}
