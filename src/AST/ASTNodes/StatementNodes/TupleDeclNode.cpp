//
// Created by csun on 11/1/18.
//

#include "AST/ASTNodes/StatementNodes/TupleDeclNode.h"

TupleDeclNode::TupleDeclNode(ASTNode *expr, std::vector<std::string> *typeID, std::vector<std::string> *identifierID,
                             bool constant, const std::string &ID) : UnaryNode(expr), typeID(typeID),
                                                                     identifierID(identifierID), constant(constant),
                                                                     ID(ID) {}
