//
// Created by ali5 on 11/18/18.
//

#include <AST/ASTNodes/TerminalNodes/IntervalNode.h>

IntervalNode::IntervalNode(ASTNode* leftBound, ASTNode* rightBound, int line) : ASTNode(line), leftBound(leftBound), rightBound(rightBound){
setType(INTERVAL);}

ASTNode* IntervalNode::getLeftBound() {
    return this->leftBound;
}

ASTNode* IntervalNode::getRightBound() {
    return this->rightBound;
}