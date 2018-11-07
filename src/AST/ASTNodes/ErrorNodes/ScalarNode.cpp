//
// Created by ali5 on 11/5/18.
//
#include "AST/ASTNodes/ErrorNodes/ScalarNode.h"

ScalarNode::ScalarNode(std::string &left, std::string &right, int line): left(left), right(right), line(line){};

std::string ScalarNode::getNode() {
    std::string errorString = "Type error: Cannot convert between " + this->left + " and " + this->right + " on line " + std::to_string(this->line) + "\n";

    return errorString;
}