//
// Created by ali5 on 11/26/18.
//

#include "AST/ASTNodes/ErrorNodes/ErrorNode.h"

ErrorNode::ErrorNode(ErrorNode::errorType etype) : eType(etype) {}

ErrorNode::errorType ErrorNode::getErrorType() const {
    return this->eType;
}