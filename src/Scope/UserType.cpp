//
// Created by kyle on 26/10/18.
//
#include "Scope/UserType.h"

UserType::UserType(const std::string &name, llvm::Type *typeDef,
                   const std::unordered_map<std::string, llvm::Type *> &members, int dim1, int dim2) : GazpreaType(name, typeDef, dim1, dim2),
                                                                                   members(members) {}

UserType::UserType(const std::string &name, llvm::Type *typeDef, int dim1, int dim2) : GazpreaType(name, typeDef, dim1, dim2) {}
