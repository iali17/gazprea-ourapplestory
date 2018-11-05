//
// Created by kyle on 04/11/18.
//

#ifndef GAZPREABASE_GLOBALDECLNODE_H
#define GAZPREABASE_GLOBALDECLNODE_H
#include "AST/ASTNodes/StatementNodes/DeclNode.h"

class GlobalDeclNode : public UnaryNode  {
public:
    GlobalDeclNode(ASTNode *expr, const std::string &ID, std::vector<std::string> *typeIds);
    const std::string &getID() const;
    std::vector<std::string> *getTypeIds() const;

protected:
    std::string ID;
    std::vector<std::string> *typeIds;
};
#endif //GAZPREABASE_GLOBALDECLNODE_H
