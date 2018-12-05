//
// Created by iali1 on 11/4/18.
//

#ifndef GAZPREABASE_PROTOPROCEDURENODE_H
#define GAZPREABASE_PROTOPROCEDURENODE_H
#include <llvm/IR/Type.h>
#include <string>
#include <AST/ASTNodes/BaseNodes/ASTNode.h>
#include <AST/ASTNodes/TypeNodes/TupleTypeNode.h>

class ProtoProcedureNode : public ASTNode {
public:

    ProtoProcedureNode(std::vector<ASTNode *> *paramNodes, const std::string &retType,
                       const std::string &procedureName, int line, TupleTypeNode *tupleType = nullptr, int gType = -1);

    std::vector<ASTNode *> *getParamNodes() const;
    const std::string &getRetType() const;
    const std::string &getProcedureName() const;
    TupleTypeNode *getTupleType() const;

    int getGType() const;

protected:
    std::vector<ASTNode *> *paramNodes;
    std::string retType;
    std::string procedureName;
    TupleTypeNode * tupleType;
    int gType;
};

#endif //GAZPREABASE_PROTOPROCEDURENODE_H
