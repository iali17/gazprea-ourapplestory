//
// Created by iali1 on 12/2/18.
//

#include <AST/ASTNodes/TerminalNodes/StringNode.h>

StringNode::StringNode(std::vector<ASTNode *> *elements, int line) :ASTNode(line), elements(elements){}

std::vector<ASTNode *>* StringNode::getElements() {
    return this->elements;
}