//
// Created by ali5 on 11/6/18.
//

#ifndef GAZPREABASE_TYPEDEFNODE_H
#define GAZPREABASE_TYPEDEFNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"
#include "AST/AST.h"
#include <string>

class TypeDefNode : public ASTNode {
public:
    TypeDefNode(const std::string &id, const std::string &type);
    TypeDefNode(const std::string &id, ASTNode *tupleType);

    const std::string &getCustomType() const;
    const std::string &getId() const;

    ASTNode *getTuple();

protected:
    std::string id;
    std::string type;
    ASTNode *tupleType = nullptr;
};

#endif //GAZPREABASE_TYPEDEFNODE_H
