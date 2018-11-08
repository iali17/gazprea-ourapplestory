//
// Created by kyle on 22/10/18.
//

#include "AST/ASTNodes/BaseNodes/BasicBlockNode.h"

BasicBlockNode::BasicBlockNode(std::vector<ASTNode *> *nodes, int line) : ASTNode(line), nodes(nodes) {}

BasicBlockNode::BasicBlockNode(int line) : ASTNode(line) {}