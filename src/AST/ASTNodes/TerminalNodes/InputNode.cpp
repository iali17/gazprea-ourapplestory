//
// Created by kyle on 27/10/18.
//

#include "AST/ASTNodes/TerminalNodes/InputNode.h"

InputNode::InputNode(const std::string &streamName, const std::string &storeID) : IONode(streamName),
                                                                                  storeID(storeID) {}

const std::string &InputNode::getStoreID() const {
    return storeID;
}
