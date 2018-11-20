//
// Created by csun on 11/20/18.
//

#ifndef GAZPREABASE_MATRIXDECLNODE_H
#define GAZPREABASE_MATRIXDECLNODE_H


#include <AST/ASTNodes/BaseNodes/UnaryNode.h>

class MatrixDeclNode : public UnaryNode{
public:
    MatrixDeclNode(ASTNode *expr, int line, bool constant, const std::string &ID, const std::string &initType,
                   ASTNode *matrixType);

    bool isConstant() const;

    const std::string &getID() const;

    const std::string &getInitType() const;

    ASTNode *getMatrixType() const;

protected:
    bool constant;
    std::string ID;
    std::string initType; // contains the string of the type of matrix (real, integer, char, etc)
    ASTNode * matrixType; // represents the matrix size in the decl
};


#endif //GAZPREABASE_MATRIXDECLNODE_H
