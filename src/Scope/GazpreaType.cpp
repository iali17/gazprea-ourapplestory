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

GazpreaType::GazpreaType(const std::string &name, llvm::Type *typeDef, int dim1, int dim2) : name(name),
                                                                                             typeDef(typeDef),
                                                                                             dim1(dim1), dim2(dim2) {}

int GazpreaType::getDim1() const {
    return dim1;
}

void GazpreaType::setDim1(int dim1) {
    GazpreaType::dim1 = dim1;
}

int GazpreaType::getDim2() const {
    return dim2;
}

void GazpreaType::setDim2(int dim2) {
    GazpreaType::dim2 = dim2;
}
