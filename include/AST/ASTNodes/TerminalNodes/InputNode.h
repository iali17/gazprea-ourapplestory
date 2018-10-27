//
// Created by kyle on 27/10/18.
//

#ifndef GAZPREABASE_INPUTNODE_H
#define GAZPREABASE_INPUTNODE_H
#include "AST/ASTNodes/BaseNodes/IONode.h"

class InputNode : public IONode {
protected:
    std::string storeID;
public:
    const std::string &getStoreID() const;
    InputNode(const std::string &streamName, const std::string &storeID);
};
#endif //GAZPREABASE_INPUTNODE_H
