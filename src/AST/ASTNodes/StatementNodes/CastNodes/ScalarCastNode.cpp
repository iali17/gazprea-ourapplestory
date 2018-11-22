//
// Created by andyli on 21/11/18.
//

#include "AST/ASTNodes/StatementNodes/CastNodes/ScalarCastNode.h"


ScalarCastNode::ScalarCastNode(ASTNode *expr, const std::string &type, int line) : CastExprNode(expr, SCALAR_CAST, line), type(type){}

const std::string &ScalarCastNode::getTypeString() const {
    return type;
}
