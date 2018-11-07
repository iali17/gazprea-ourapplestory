//
// Created by kyle on 04/11/18.
//
#include "AST/AST.h"

GlobalRefNode::GlobalRefNode(const std::string &globalName) : globalName(globalName) {}

const std::string &GlobalRefNode::getGlobalName() const {
    return globalName;
}
