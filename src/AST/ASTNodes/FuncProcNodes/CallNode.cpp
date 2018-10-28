//
// Created by iali1 on 10/28/18.
//

#include <string>
#include <AST/ASTNodes/FuncProcNodes/CallNode.h>

CallNode::CallNode(std::vector<ASTNode *> *exprNodes, const std::string &procedureName) : exprNodes(exprNodes),
                                                                                          procedureName(
                                                                                                  procedureName) {}

std::vector<ASTNode *> *CallNode::getExprNodes() const {
    return exprNodes;
}

const std::string &CallNode::getProcedureName() const {
    return procedureName;
}
