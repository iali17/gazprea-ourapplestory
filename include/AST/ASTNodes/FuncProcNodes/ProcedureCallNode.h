//
// Created by iali on 11/2/18.
//
#ifndef GAZPREABASE_PROCEDURECALLNODE_H
#define GAZPREABASE_PROCEDURECALLNODE_H
#include "AST/ASTNodes/BaseNodes/ASTNode.h"

#define MINUS 1
#define PLUS 2
#define NEG 3

class ProcedureCallNode : public ASTNode {
public:
    ProcedureCallNode(const std::string &varName, const std::string &procedureName, std::vector<ASTNode *> *exprNode,
                      std::vector<std::string> *typeIds, bool constant, int line, int unOP = PLUS);

    ProcedureCallNode(const std::string &varName, const std::string &procedureName, std::vector<ASTNode *> *exprNode,
            int line, int unOP = PLUS);

    const std::string &getVarName() const;
    std::vector<ASTNode *> *getExprNode() const;
    const std::string &getProcedureName() const;
    std::vector<std::string> *getTypeIds() const;
    int getUnOp() const;
    bool isConstant() const;

protected:
    std::string varName;
    std::string procedureName;
    int unOp;
    std::vector<ASTNode *> *exprNode = new std::vector<ASTNode *>;
    std::vector<std::string> *typeIds = new std::vector<std::string>;
    bool constant;
};
#endif //GAZPREABASE_PROCEDURECALLNODE_H