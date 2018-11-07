//
// Created by kyle on 27/10/18.
//

#ifndef GAZPREABASE_TYPE_H
#define GAZPREABASE_TYPE_H
#include <llvm/IR/Type.h>

class GazpreaType {
public:
    GazpreaType(const std::string &name, llvm::Type *typeDef);
    const std::string &getName() const;
    llvm::Type *getTypeDef() const;

protected:
    std::string name;
    llvm::Type* typeDef;
};


#endif //GAZPREABASE_TYPE_H
