//
// Created by kyle on 27/10/18.
//

#include "Scope/UserType.h"
#include "Scope/GazpreaType.h"

const std::string &GazpreaType::getName() const {
    return name;
}

llvm::Type *GazpreaType::getTypeDef() const {
    return typeDef;
}

GazpreaType::GazpreaType(const std::string &name, llvm::Type *typeDef) : name(name), typeDef(typeDef) {}