//
// Created by csun on 11/5/18.
//
#include <AST/ASTNodes/StatementNodes/PythonTupleAssNode.h>

PythonTupleAssNode::PythonTupleAssNode(ASTNode *expr, const std::vector<std::string> &IDs, int line) : UnaryNode(expr),
                                                                                             IDs(IDs), line(line) {}

const std::vector<std::string> &PythonTupleAssNode::getIDs() const {
    return IDs;
}

int PythonTupleAssNode::getLine() {
    return this->line;
}