//
// Created by csun on 11/20/18.
//

#include <AST/ASTNodes/TerminalNodes/MatrixNode.h>

#include "AST/ASTNodes/TerminalNodes/MatrixNode.h"

MatrixNode::MatrixNode(std::vector<ASTNode *> *insideElement, int line)
: ASTNode(line), insideElement(insideElement) {
    type = MATRIX;
}

std::vector<ASTNode *> *MatrixNode::getElements() {
    return insideElement;
}
