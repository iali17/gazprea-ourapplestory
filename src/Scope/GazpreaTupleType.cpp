//
// Created by lepoidev on 11/4/18.
//
#include <Scope/GazpreaTupleType.h>

GazpreaTupleType::GazpreaTupleType(const std::string &name, llvm::Type *typeDef,
                         std::unordered_map<std::string, int> *stringRefMap, std::vector<llvm::Type *> *members)
        : GazpreaType(name, typeDef), stringRefMap(stringRefMap), members(members) {}

int GazpreaTupleType::getOffsetFromString(std::string str) {
    return stringRefMap->at(str);
}

std::vector<llvm::Type *> *GazpreaTupleType::getMembers() const {
    return members;
}

std::unordered_map<std::string, int> *GazpreaTupleType::getStringRefMap() const {
    return stringRefMap;
}
