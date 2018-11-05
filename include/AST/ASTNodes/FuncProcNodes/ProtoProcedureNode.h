//
// Created by iali1 on 11/4/18.
//

#ifndef GAZPREABASE_PROTOPROCEDURENODE_H
#define GAZPREABASE_PROTOPROCEDURENODE_H

#include <llvm/IR/Type.h>
#include "../../AST.h"
#include <string>

class ProtoProcedureNode : public ASTNode {
public:

    ProtoProcedureNode(std::vector<ASTNode *> *paramNodes, const std::string &retType,
                       const std::string &procedureName);

    std::vector<ASTNode *> *getParamNodes() const;

    const std::string &getRetType() const;

    const std::string &getProcedureName() const;

protected:
    std::vector<ASTNode *> *paramNodes;
    std::string retType;
    std::string procedureName;
};

#endif //GAZPREABASE_PROTOPROCEDURENODE_H
