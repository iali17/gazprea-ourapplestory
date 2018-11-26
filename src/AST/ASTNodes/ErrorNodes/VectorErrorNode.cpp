//
// Created by ali5 on 11/26/18.
//

#include <AST/ASTNodes/ErrorNodes/VectorErrorNode.h>

#include "AST/ASTNodes/ErrorNodes/VectorErrorNode.h"

VectorErrorNode::VectorErrorNode(std::string &left, std::string &right, int leftSize, int rightSize, int line): ErrorNode(VECTOR_ERROR), left(left), right(right), leftSize(leftSize), rightSize(rightSize), line(line){
    this->leftExprSize = true;
    this->nsError = false;
    this->stv = false;
};

VectorErrorNode::VectorErrorNode(std::string &left, std::string &right, int vectorSize, bool leftExprSize, int line): ErrorNode(VECTOR_ERROR), left(left), right(right), leftExprSize(leftExprSize), line(line) {
    if(leftExprSize)
        this->leftSize = vectorSize;
    else
        this->rightSize = vectorSize;

    this->nsError = false;
    this->stv = true;
};

VectorErrorNode::VectorErrorNode(std::string &left, std::string &right, int line): ErrorNode(VECTOR_ERROR), left(left), right(right), line(line) {
    this->leftExprSize = false;
    this->nsError = true;
    this->stv = true;
}

bool VectorErrorNode::isStv() {
    return this->stv;
}

bool VectorErrorNode::isNoSizeError(){
    return this->nsError;
}

std::string VectorErrorNode::getNode() {
    std::string errorString = "Type error: Cannot convert between " + this->left + " vector[" + std::to_string(this->leftSize) + "] and "
                              + this->right + " vector[" + std::to_string(this->rightSize) + "] on line " + std::to_string(this->line) + "\n";

    return errorString;
}

std::string VectorErrorNode::getSTVNode() {
    std::string errorString;

    if(leftExprSize) {
        errorString = "Type error: Cannot convert between " + this->left + " vector[" + std::to_string(this->leftSize) + "] and "
                      + this->right + " on line " + std::to_string(this->line) + "\n";
    } else {
        errorString = "Type error: Cannot convert between " + this->left + " vector and "
                      + this->right + " vector[" + std::to_string(this->rightSize) + "] on line " + std::to_string(this->line) + "\n";
    }

    return errorString;
}

std::string VectorErrorNode::getSTVNSNode() {
    std::string errorString;

    errorString = "Type error: cannot convert between " + this->left + " vector" + " and " + this->right + " on line " + std::to_string(this->line) + "\n";

    return errorString;
}
