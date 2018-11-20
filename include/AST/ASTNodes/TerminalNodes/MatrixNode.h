//
// Created by csun on 11/20/18.
//

#ifndef GAZPREABASE_MATRIXNODE_H
#define GAZPREABASE_MATRIXNODE_H


#include <AST/ASTNodes/BaseNodes/ASTNode.h>

class MatrixNode : public ASTNode{
public:
    MatrixNode(std::vector<ASTNode *> *insideElement, int line);
    std::vector<ASTNode*>* getElements();

protected:
    std::vector<ASTNode*>* insideElement;

};


#endif //GAZPREABASE_MATRIXNODE_H
