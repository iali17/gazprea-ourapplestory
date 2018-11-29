//
// Created by kyle on 08/11/18.
//

#ifndef GAZPREABASE_FUNCTIONNODE_H
#define GAZPREABASE_FUNCTIONNODE_H

#include <AST/ASTNodes/TypeNodes/TupleTypeNode.h>
#include "AST/ASTNodes/BaseNodes/BaseBlockParent.h"

class FunctionNode : public BaseBlockParent {
public:
    FunctionNode(BlockNode *blockNode, std::vector<ASTNode *> *paramNodes, const std::string &retType,
    const std::string &functionName, int line, TupleTypeNode *tupleType = nullptr, int gType = -1);

    std::vector<ASTNode *> *getParamNodes() const;
    const std::string &getRetType() const;
    const std::string &getFunctionName() const;
    TupleTypeNode *getTupleType() const;

    int getGType() const;

protected:
    std::vector<ASTNode *> *paramNodes;
    std::string retType;
    std::string functionName;
    TupleTypeNode * tupleType;
    int gType;
};
#endif //GAZPREABASE_FUNCTIONNODE_H
