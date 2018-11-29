//
// Created by csun on 11/4/18.
//
#include <AST/ASTNodes/TypeNodes/TupleTypeNode.h>


TupleTypeNode::TupleTypeNode(std::vector<ASTNode *> *decls, int line) : ASTNode(line), decls(decls){

}

std::vector<ASTNode *> *TupleTypeNode::getDecls() const {
    return decls;
}

