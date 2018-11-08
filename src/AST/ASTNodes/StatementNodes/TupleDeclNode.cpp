#include <AST/ASTNodes/StatementNodes/TupleDeclNode.h>

TupleDeclNode::TupleDeclNode(ASTNode *expr, bool constant, const std::string &ID, ASTNode *tupleTypes, int line)
: UnaryNode(expr, line), constant(constant), ID(ID), tupleTypes(tupleTypes) {}


bool TupleDeclNode::isConstant() const {
    return constant;
}

const std::string &TupleDeclNode::getID() const {
    return ID;
}

ASTNode *TupleDeclNode::getTupleTypes() const {
    return tupleTypes;
}



