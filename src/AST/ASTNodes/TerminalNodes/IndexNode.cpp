//
// Created by ali5 on 11/18/18.
//

#include <AST/ASTNodes/TerminalNodes/IndexNode.h>

IndexNode::IndexNode(std::vector<ASTNode *> *exprIndex, int indexSize, int line) : ASTNode(line), indexSize(indexSize), exprIndex(exprIndex) {}

std::vector<ASTNode *>* IndexNode::getExprs() {
    return this->exprIndex;
}

int IndexNode::getIndexSize() {
    return this->indexSize;
}