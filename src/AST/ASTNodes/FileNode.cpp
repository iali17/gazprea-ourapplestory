//
// Created by kyle on 20/10/18.
//
#include "AST/ASTNodes/FileNode.h"

FileNode::FileNode(int line) : BasicBlockNode(line) {}

FileNode::FileNode(std::vector<ASTNode *> *nodes, int line) : BasicBlockNode(nodes, line) {}
