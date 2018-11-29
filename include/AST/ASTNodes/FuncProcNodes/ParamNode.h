//
// Created by kyle on 22/10/18.
//

#ifndef GAZPREABASE_PARAMNODE_H
#define GAZPREABASE_PARAMNODE_H

#include <AST/ASTNodes/TypeNodes/TupleTypeNode.h>
#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class ParamNode : public ASTNode {
public:
    ParamNode(const std::string &declaredType, const std::string &varName, bool isVar, int line, TupleTypeNode *tupleType = nullptr, int gType = -1);

    const std::string &getDeclaredType() const;

    const std::string &getVarName() const;

    bool isIsVar() const;

    int getGType() const;

protected:
    std::string declaredType;
    std::string varName;
    bool isVar;
public:
    TupleTypeNode *getTupleType() const;

protected:
    TupleTypeNode *tupleType;
    int gType;
};

#endif //GAZPREABASE_PARAMNODE_H
