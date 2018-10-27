//
// Created by kyle on 26/10/18.
//

#include "Scope/UserType.h"

UserType::UserType(const std::string &name, llvm::Type *typeDef,
                   const std::unordered_map<std::string, llvm::Type *> &members) : GazpreaType(name, typeDef),
                                                                                   members(members) {}

UserType::UserType(const std::string &name, llvm::Type *typeDef) : GazpreaType(name, typeDef) {}
