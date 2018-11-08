//
// Created by iali on 10/22/18.
//

#ifndef GAZPREABASE_IDNODE_H
#define GAZPREABASE_IDNODE_H
#include <string>
#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class IDNode : public ASTNode {
public:
    explicit IDNode(int line);
    explicit IDNode(const std::string &ID, int line);
    IDNode(int type, const std::string &ID, int line);
    const std::string &getID() const;

protected:
    std::string ID;
};
#endif //GAZPREABASE_IDNODE_H
