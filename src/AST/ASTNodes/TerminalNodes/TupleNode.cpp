//
// Created by csun on 11/1/18.
//
#include <AST/ASTNodes/TerminalNodes/TupleNode.h>

TupleNode::TupleNode(std::vector<ASTNode *> *insideElement, int line) : ASTNode(line), insideElement(insideElement) {
    type = TUPLE;
}

std::vector<ASTNode *> *TupleNode::getElements() {
    return insideElement;
}
