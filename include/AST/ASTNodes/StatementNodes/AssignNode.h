//
// Created by iali on 10/22/18.
//

#ifndef GAZPREABASE_ASSIGNNODE_H
#define GAZPREABASE_ASSIGNNODE_H

#include <string>
#include "AST/ASTNodes/BaseNodes/UnaryNode.h"


class AssignNode : public UnaryNode  {
public:
    AssignNode(ASTNode *expr, const std::string &ID);

    const std::string &getID() const;


protected:
    std::string ID;
};

#endif //GAZPREABASE_ASSIGNNODE_H
