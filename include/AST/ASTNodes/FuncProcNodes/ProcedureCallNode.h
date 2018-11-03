//
// Created by iali on 11/2/18.
//

#ifndef GAZPREABASE_PROCEDURECALLNODE_H
#define GAZPREABASE_PROCEDURECALLNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"
#include <string>

class ProcedureCallNode : public ASTNode {
public:
    ProcedureCallNode(const std::string &varName, const std::string &procedureName, std::vector<ASTNode *> *exprNode,
                      std::vector<std::string> *typeIds, bool constant);
    const std::string &getVarName() const;
    std::vector<ASTNode *> *getExprNode() const;
    const std::string &getProcedureName() const;
    std::vector<std::string> *getTypeIds() const;
    bool isConstant() const;

protected:
    std::string varName;
    std::string procedureName;
    std::vector<ASTNode *> *exprNode;
    std::vector<std::string> *typeIds;
    bool constant;
};

#endif //GAZPREABASE_PROCEDURECALLNODE_H
