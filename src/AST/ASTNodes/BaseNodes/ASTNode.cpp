//
// Created by kyle on 20/10/18.
//

#include <AST/ASTNodes/BaseNodes/ASTNode.h>

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
