//
// Created by csun on 11/20/18.
//

#ifndef GAZPREABASE_MATRIXTYPE_H
#define GAZPREABASE_MATRIXTYPE_H


#include <AST/ASTNodes/BaseNodes/ASTNode.h>

class MatrixTypeNode : ASTNode{
public:
    MatrixTypeNode(ASTNode *left, ASTNode *right, int line, const std::string &stringType);
    ASTNode *getLeft() const;
    ASTNode *getRight() const;
    const std::string &getStringType() const;

protected:
    ASTNode * left;
    ASTNode * right;
    std::string stringType;
};


#endif //GAZPREABASE_MATRIXTYPE_H
