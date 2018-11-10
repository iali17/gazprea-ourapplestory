//
// Created by kyle on 29/10/18.
//

#include <AST/ASTNodes/BaseNodes/ASTNode.h>
#include <AST/ASTNodes/BaseNodes/BitArithNode.h>

BitArithNode::BitArithNode(ASTNode *left, ASTNode *right, int line) : InfixNode(left, right, line) {
    setType(BOOLEAN);
}
