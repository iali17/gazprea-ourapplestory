//
// Created by ali5 on 11/18/18.
//

#include <AST/ASTNodes/TerminalNodes/IndexNode.h>

IndexNode::IndexNode(ASTNode *LHS, std::vector<ASTNode *> *indexExpr, int line)
    : ASTNode(line), LHS (LHS), indexExpr(indexExpr) {}

ASTNode *IndexNode::getLHS() const {
    return LHS;
}

std::vector<ASTNode *> *IndexNode::getIndexExpr() const {
    return indexExpr;
}
