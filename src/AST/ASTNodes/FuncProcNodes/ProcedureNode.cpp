//
// Created by kyle on 22/10/18.
//
#include <AST/ASTNodes/FuncProcNodes/ProcedureNode.h>

const std::string &ProcedureNode::getRetType() const {
    return retType;
}

const std::string &ProcedureNode::getProcedureName() const {
    return procedureName;
}

std::vector<ASTNode *> *ProcedureNode::getParamNodes() const {
    return paramNodes;
}

ProcedureNode::ProcedureNode(BlockNode *blockNode, std::vector<ASTNode *> *paramNodes, const std::string &retType,
                             const std::string &procedureName, int line) : BaseBlockParent(blockNode, line), paramNodes(paramNodes),
                                                                 retType(retType), procedureName(procedureName) {}

