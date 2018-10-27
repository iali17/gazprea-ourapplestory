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
    UserType(const std::string &name, llvm::Type *typeDef);

    UserType(const std::string &name, llvm::Type *typeDef,
             const std::unordered_map<std::string, llvm::Type *> &members);

protected:
    std::unordered_map<std::string, llvm::Type* > members;
};
#endif //GAZPREABASE_USERTYPE_H
