//
// Created by csun on 11/22/18.
//



#include <AST/ASTNodes/StatementNodes/IntervalDeclNode.h>

IntervalDeclNode::IntervalDeclNode(ASTNode *expr, bool constant, const std::string &ID, int line)
: UnaryNode(expr, line), constant(constant), ID(ID) {

}

bool IntervalDeclNode::isConstant() const {
    return constant;
}

const std::string &IntervalDeclNode::getID() const {
    return ID;
}
