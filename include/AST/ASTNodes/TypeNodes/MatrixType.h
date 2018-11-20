//
// Created by csun on 11/20/18.
//

#ifndef GAZPREABASE_MATRIXTYPE_H
#define GAZPREABASE_MATRIXTYPE_H


#include <AST/ASTNodes/BaseNodes/ASTNode.h>

class MatrixType : ASTNode{
public:
    MatrixType(ASTNode *left, ASTNode *right, int line);
    ASTNode *getLeft() const;
    ASTNode *getRight() const;

protected:
    ASTNode * left;
    ASTNode * right;
};


#endif //GAZPREABASE_MATRIXTYPE_H
