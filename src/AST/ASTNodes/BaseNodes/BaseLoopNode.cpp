//
// Created by kyle on 26/10/18.
//

#include "AST/ASTNodes/BaseNodes/BaseLoopNode.h"

ASTNode *BaseLoopNode::getControl() const {
    return control;
}

BaseLoopNode::BaseLoopNode(ASTNode *control, int line) : BaseBlockParent(line), control(control) {}

BaseLoopNode::BaseLoopNode(BlockNode *blockNode, ASTNode *control, int line) : BaseBlockParent(blockNode, line), control(control) {}
