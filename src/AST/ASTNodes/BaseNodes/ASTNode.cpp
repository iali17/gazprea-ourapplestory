//
// Created by kyle on 20/10/18.
//

#include <AST/ASTNodes/BaseNodes/ASTNode.h>

ASTNode::ASTNode(int line) : line(line) {}

int ASTNode::getType() const {
    return type;
}

void ASTNode::setType(int newType){
    type = newType;
}

llvm::Type *ASTNode::getLlvmType() const {
    return llvmType;
}

void ASTNode::setLlvmType(llvm::Type *llvmType) {
    ASTNode::llvmType = llvmType;
}

int ASTNode::getLine() const {
    return line;
}

void ASTNode::setLine(int linee) {
    line = linee;
}


