//
// Created by ali5 on 11/20/18.
//

#include <AST/ASTNodes/TerminalNodes/VectorNode.h>

VectorNode::VectorNode(std::vector<ASTNode *> *elements, int line) : ASTNode(line), elements(elements) {
    type = VECTOR_T;
}

std::vector<ASTNode *>* VectorNode::getElements() {
    return this->elements;
}

