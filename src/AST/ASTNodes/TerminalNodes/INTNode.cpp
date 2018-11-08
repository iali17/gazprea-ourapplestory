//
// Created by iali on 10/22/18.
//
#include <AST/ASTNodes/TerminalNodes/INTNode.h>

INTNode::INTNode(int value, int line) : value(value), ASTNode(line) {
setType(INTEGER);}