//
// Created by kyle on 26/10/18.
//

#ifndef GAZPREABASE_USERTYPE_H
#define GAZPREABASE_USERTYPE_H
#include <unordered_map>
#include <llvm/IR/Type.h>

class UserType {
public:
    UserType(const std::string &name, llvm::Type *typeDef,
             const std::unordered_map<std::string, llvm::Type *> &members);
    UserType(const std::string &name, llvm::Type *typeDef);

    const std::string &getName() const;

    llvm::Type *getTypeDef() const;

protected:
    std::string name;
    llvm::Type* typeDef;
    std::unordered_map<std::string, llvm::Type* > members;
};
#endif //GAZPREABASE_USERTYPE_H
