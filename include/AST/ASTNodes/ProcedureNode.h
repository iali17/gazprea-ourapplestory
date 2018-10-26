//
// Created by kyle on 22/10/18.
//

#ifndef GAZPREABASE_PROCEDURENODE_H
#define GAZPREABASE_PROCEDURENODE_H

#include <llvm/IR/Type.h>
#include "BaseNodes/BasicBlockNode.h"
#include "BaseNodes/BlockNode.h"
#include "ParamNode.h"

class ProcedureNode : public ASTNode {
public:
    ProcedureNode(std::vector<ASTNode *> *paramNodes, BlockNode *blockNode, const std::string &retType,
                  const std::string &procedureName);

    BlockNode *getBlockNode() const;
    BasicBlockNode *getFullBlock();
    const std::string &getRetType() const;
    const std::string &getProcedureName() const;

    std::vector<ASTNode *> *getParamNodes() const;

protected:
    std::vector<ASTNode *> *paramNodes;
    BlockNode *blockNode;
    std::string retType;
    std::string procedureName;
};
#endif //GAZPREABASE_PROCEDURENODE_H
