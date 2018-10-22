//
// Created by iali on 10/22/18.
//

#ifndef GAZPREABASE_IDNODE_H
#define GAZPREABASE_IDNODE_H

#include <string>
#include "ASTNode.h"

class IDNode : public ASTNode {
public:
    const std::string &getID() const;
    IDNode();

    IDNode(int type, const std::string &ID);

protected:
    std::string ID;
};
#endif //GAZPREABASE_IDNODE_H
