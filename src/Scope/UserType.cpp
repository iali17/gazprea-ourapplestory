//
// Created by kyle on 26/10/18.
//

#include "Scope/UserType.h"

UserType::UserType(const std::string &name, llvm::Type *typeDef) : name(name), typeDef(typeDef) {}

UserType::UserType(const std::string &name, llvm::Type *typeDef,
                   const std::unordered_map<std::string, llvm::Type *> &members) : name(name), typeDef(typeDef),
                                                                                   members(members) {}

const std::string &UserType::getName() const {
    return name;
}

llvm::Type *UserType::getTypeDef() const {
    return typeDef;
}
