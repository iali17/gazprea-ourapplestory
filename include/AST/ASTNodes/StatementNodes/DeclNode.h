//
// Created by iali on 10/22/18.
//

#ifndef GAZPREABASE_DECLNODE_H
#define GAZPREABASE_DECLNODE_H
#include <string>
#include "AST/ASTNodes/BaseNodes/UnaryNode.h"

class DeclNode : public UnaryNode  {
public:
    const std::string &getID() const;
    DeclNode(ASTNode *expr, bool constant, const std::string &ID, std::vector<std::string> *typeIds, int newType, int line);
    bool isConstant() const;
    std::vector<std::string> *getTypeIds() const;

protected:
    bool constant;
    std::string ID;
    std::vector<std::string> *typeIds;
};


#endif //GAZPREABASE_DECLNODE_H
