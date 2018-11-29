//
// Created by kyle on 22/10/18.
//

#ifndef GAZPREABASE_PROCEDURENODE_H
#define GAZPREABASE_PROCEDURENODE_H

#include <llvm/IR/Type.h>
#include "AST/ASTNodes/BaseNodes/BaseBlockParent.h"
#include "ParamNode.h"

class ProcedureNode : public BaseBlockParent {
public:
    ProcedureNode(BlockNode *blockNode, std::vector<ASTNode *> *paramNodes, const std::string &retType,
                  const std::string &procedureName, int line, TupleTypeNode *tupleType = nullptr, int gType = -1);

    const std::string &getRetType() const;
    const std::string &getProcedureName() const;

    std::vector<ASTNode *> *getParamNodes() const;

    TupleTypeNode *getTupleType() const;

    int getGType() const;

protected:
    std::vector<ASTNode *> *paramNodes;
    std::string retType;
    std::string procedureName;
    TupleTypeNode * tupleType;
    int gType;
};
#endif //GAZPREABASE_PROCEDURENODE_H
