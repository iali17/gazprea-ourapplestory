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