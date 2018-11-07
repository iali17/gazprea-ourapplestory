//
// Created by kyle on 04/11/18.
//

#ifndef GAZPREABASE_GLOBALREFNODE_H
#define GAZPREABASE_GLOBALREFNODE_H
#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class GlobalRefNode : public ASTNode {
public:
    explicit GlobalRefNode(const std::string &globalName);
    const std::string &getGlobalName() const;

protected:
    std::string globalName;
};

#endif //GAZPREABASE_GLOBALREFNODE_H
