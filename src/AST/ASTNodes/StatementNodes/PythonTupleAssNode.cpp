//
// Created by csun on 11/5/18.
//
#include <AST/ASTNodes/StatementNodes/PythonTupleAssNode.h>

PythonTupleAssNode::PythonTupleAssNode(ASTNode *expr, const std::vector<std::string> &IDs) : UnaryNode(expr),
                                                                                             IDs(IDs) {}

const std::vector<std::string> &PythonTupleAssNode::getIDs() const {
    return IDs;
}
