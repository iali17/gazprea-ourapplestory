//
// Created by kyle on 08/11/18.
//

#ifndef GAZPREABASE_FUNCTIONNODE_H
#define GAZPREABASE_FUNCTIONNODE_H

#include <AST/ASTNodes/TypeNodes/TupleType.h>
#include "AST/ASTNodes/BaseNodes/BaseBlockParent.h"

class FunctionNode : public BaseBlockParent {
public:
    FunctionNode(BlockNode *blockNode, std::vector<ASTNode *> *paramNodes, const std::string &retType,
    const std::string &functionName, int line, TupleType *tupleType = nullptr);

    std::vector<ASTNode *> *getParamNodes() const;
    const std::string &getRetType() const;
    const std::string &getFunctionName() const;

protected:
    std::vector<ASTNode *> *paramNodes;
public:
    TupleType *getTupleType() const;

protected:
    std::string retType;
    std::string functionName;
    TupleType * tupleType;
};
#endif //GAZPREABASE_FUNCTIONNODE_H
