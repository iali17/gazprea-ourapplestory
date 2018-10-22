//
// Created by kyle on 20/10/18.
//
#include "AST/ASTNodes/FileNode.h"

FileNode::FileNode() {}

FileNode::FileNode(std::vector<ASTNode *> *nodes) : BasicBlockNode(nodes) {}
