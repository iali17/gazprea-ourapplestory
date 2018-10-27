//
// Created by kyle on 26/10/18.
//

#include "AST/ASTNodes/BaseNodes/BaseLoopNode.h"

BaseLoopNode::BaseLoopNode(ASTNode *body) : body(body) {
    control = nullptr;
}

BaseLoopNode::BaseLoopNode(ASTNode *body, ASTNode *control) : body(body), control(control) {}

ASTNode *BaseLoopNode::getBody() const {
    return body;
}

ASTNode *BaseLoopNode::getControl() const {
    return control;
}
