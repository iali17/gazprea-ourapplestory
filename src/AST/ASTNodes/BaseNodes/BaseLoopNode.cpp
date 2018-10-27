//
// Created by kyle on 26/10/18.
//

#include "AST/ASTNodes/BaseNodes/BaseLoopNode.h"

ASTNode *BaseLoopNode::getControl() const {
    return control;
}

BaseLoopNode::BaseLoopNode(ASTNode *control) : control(control) {}

BaseLoopNode::BaseLoopNode(BlockNode *blockNode, ASTNode *control) : BaseBlockParent(blockNode), control(control) {}
