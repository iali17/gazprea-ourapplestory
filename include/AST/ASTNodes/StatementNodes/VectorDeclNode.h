//
// Created by ali5 on 11/20/18.
//

#ifndef GAZPREABASE_VECTORDECLNODE_H
#define GAZPREABASE_VECTORDECLNODE_H

#include <AST/ASTNodes/BaseNodes/UnaryNode.h>
#include <string>

class VectorDeclNode : public UnaryNode {
public:
    VectorDeclNode(ASTNode *expr, bool constant, const std::string &ID, ASTNode *size, int line);
    VectorDeclNode(ASTNode *expr, bool constant, const std::string &ID, int line);

    const std::string &getID() const;
    bool isConstant() const;
    ASTNode *getSize();

protected:
    bool constant;
    std::string ID;
    ASTNode *size = nullptr;

};

#endif //GAZPREABASE_VECTORDECLNODE_H
