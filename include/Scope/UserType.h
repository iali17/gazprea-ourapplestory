//
// Created by kyle on 26/10/18.
//

#ifndef GAZPREABASE_USERTYPE_H
#define GAZPREABASE_USERTYPE_H
#include <unordered_map>
#include <llvm/IR/Type.h>
#include "GazpreaType.h"

class UserType : public GazpreaType {
public:
    UserType(const std::string &name, llvm::Type *typeDef, int dim1 = -1, int dim2 = -1);

    UserType(const std::string &name, llvm::Type *typeDef,
             const std::unordered_map<std::string, llvm::Type *> &members, int dim1 = -1, int dim2 = -1);

protected:
    std::unordered_map<std::string, llvm::Type* > members;
};
#endif //GAZPREABASE_USERTYPE_H
