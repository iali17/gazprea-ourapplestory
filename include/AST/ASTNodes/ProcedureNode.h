//
// Created by kyle on 22/10/18.
//

#ifndef GAZPREABASE_PROCEDURENODE_H
#define GAZPREABASE_PROCEDURENODE_H

#include <llvm/IR/Type.h>
#include "BaseNodes/BasicBlockNode.h"
#include "BaseNodes/BlockNode.h"
#include "ParamNode.h"

class ProcedureNode {
protected:
public:
    ProcedureNode(ParamNode *paramNode, BlockNode *blockNode, const std::string &retType,
                  const std::string &procedureName);

protected:
    ParamNode *paramNode;
    BlockNode *blockNode;
    std::string retType;
    std::string procedureName;
};
#endif //GAZPREABASE_PROCEDURENODE_H
