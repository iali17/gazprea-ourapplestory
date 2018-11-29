//
// Created by ali5 on 11/20/18.
//

#ifndef GAZPREABASE_VECTORTYPE_H
#define GAZPREABASE_VECTORTYPE_H

#include <AST/ASTNodes/BaseNodes/ASTNode.h>
#include <string>

class VectorTypeNode : public ASTNode{
public:
    VectorTypeNode(ASTNode *size, int line);
    VectorTypeNode(ASTNode *size, const std::string &stringType, int line);

    ASTNode *getSize() const;
    const std::string &getStringType() const;

protected:
    ASTNode * size;
    std::string stringType;
};

#endif //GAZPREABASE_VECTORTYPE_H
