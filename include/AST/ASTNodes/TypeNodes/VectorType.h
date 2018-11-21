//
// Created by ali5 on 11/20/18.
//

#ifndef GAZPREABASE_VECTORTYPE_H
#define GAZPREABASE_VECTORTYPE_H

#include <AST/ASTNodes/BaseNodes/ASTNode.h>
#include <string>

class VectorType : public ASTNode{
public:
    VectorType(ASTNode *size, int line);
    ASTNode *getSize() const;

protected:
    ASTNode * size;
};

#endif //GAZPREABASE_VECTORTYPE_H
