//
// Created by iali on 11/2/18.
//

#ifndef GAZPREABASE_PROCEDURECALLNODE_H
#define GAZPREABASE_PROCEDURECALLNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"
#include <string>
#include "AST/ASTNodes/FuncProcNodes/ParamNode.h"

class ProcedureCallNode : public ASTNode {
public:
    ProcedureCallNode(const std::string &varName, const std::string &procedureName, std::vector<ParamNode *> *exprNode,
                      std::vector<std::string> *typeIds, bool constant);
    ProcedureCallNode(const std::string &varName, const std::string &procedureName, std::vector<ParamNode *> *exprNode);

    const std::string &getVarName() const;
    std::vector<ParamNode *> *getExprNode() const;
    const std::string &getProcedureName() const;
    std::vector<std::string> *getTypeIds() const;
    bool isConstant() const;

protected:
    std::string varName;
    std::string procedureName;
    std::vector<ParamNode *> *exprNode;
    std::vector<std::string> *typeIds;
    bool constant;
};

#endif //GAZPREABASE_PROCEDURECALLNODE_H
