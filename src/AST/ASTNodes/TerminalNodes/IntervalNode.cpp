//
// Created by ali5 on 11/18/18.
//

#include <AST/ASTNodes/TerminalNodes/IntervalNode.h>

IntervalNode::IntervalNode(int leftBound, int rightBound, int line) : ASTNode(line), leftBound(leftBound), rightBound(rightBound){

}

int IntervalNode::getLeftBound() {
    return this->leftBound;
}

int IntervalNode::getRightBound() {
    return this->rightBound;
}