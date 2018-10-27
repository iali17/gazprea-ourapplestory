//
// Created by iali on 10/22/18.
//

#ifndef GAZPREABASE_DECLNODE_H
#define GAZPREABASE_DECLNODE_H

#include <string>
#include "AST/ASTNodes/BaseNodes/UnaryNode.h"

class DeclNode : public UnaryNode  {
public:
    DeclNode(ASTNode *expr, const std::string &ID, const std::string &type_id, int newType);
    const std::string &getID() const;

protected:
    std::string ID;
    std::string type_id;
};


#endif //GAZPREABASE_DECLNODE_H
