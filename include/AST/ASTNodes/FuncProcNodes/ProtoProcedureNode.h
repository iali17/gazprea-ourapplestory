//
// Created by iali1 on 11/4/18.
//

#ifndef GAZPREABASE_PROTOPROCEDURENODE_H
#define GAZPREABASE_PROTOPROCEDURENODE_H
#include <llvm/IR/Type.h>
#include <string>
#include <AST/ASTNodes/BaseNodes/ASTNode.h>
#include <AST/ASTNodes/TypeNodes/TupleType.h>

class ProtoProcedureNode : public ASTNode {
public:

    ProtoProcedureNode(std::vector<ASTNode *> *paramNodes, const std::string &retType,
                       const std::string &procedureName, int line, TupleType *tupleType = nullptr);

    std::vector<ASTNode *> *getParamNodes() const;
    const std::string &getRetType() const;
    const std::string &getProcedureName() const;

protected:
    std::vector<ASTNode *> *paramNodes;
    std::string retType;
    std::string procedureName;
public:
    TupleType *getTupleType() const;

protected:
    TupleType * tupleType;
};

#endif //GAZPREABASE_PROTOPROCEDURENODE_H
