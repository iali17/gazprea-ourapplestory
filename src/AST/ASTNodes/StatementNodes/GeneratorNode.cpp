//
// Created by lepoidev on 11/23/18.
//

#include "AST/AST.h"

std::vector<std::string> *GeneratorNode::getLoopVars() const {
    return loopVars;
}

std::vector<ASTNode *> *GeneratorNode::getRanges() const {
    return ranges;
}

ASTNode *GeneratorNode::getExprNode() const {
    return exprNode;
}

GeneratorNode::GeneratorNode(int line, std::vector<std::string> *loopVars, std::vector<ASTNode *> *ranges, ASTNode *condNode)
        : ASTNode(line), loopVars (loopVars), ranges(ranges), exprNode(condNode) {
    setType(VECTOR);
}
