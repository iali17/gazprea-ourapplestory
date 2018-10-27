//
// Created by iali on 10/22/18.
//

#ifndef GAZPREABASE_IDNODE_H
#define GAZPREABASE_IDNODE_H

#include <string>
#include "../BaseNodes/ASTNode.h"

class IDNode : public ASTNode {
public:
    const std::string &getID() const;
    IDNode();

    IDNode(const std::string &ID);

    IDNode(int type, const std::string &ID);

protected:
    std::string ID;
};
#endif //GAZPREABASE_IDNODE_H
