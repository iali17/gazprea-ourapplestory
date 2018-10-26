//
// Created by kyle on 08/10/18.
//

#include <string>
#include <llvm/IR/Value.h>

#ifndef VCALCBASE_SYMBOL_H
#define VCALCBASE_SYMBOL_H

// Pretty much taken from the notes and refactored for C++
class Symbol {
public:
    Symbol(const std::string &scopeName, const std::string &name, int type);
    llvm::Value *getPtr() const;
    void setPtr(llvm::Value *ptr);
    int getType() const;
    llvm::Type *getLLVMType() const;
    void setLLVMType(llvm::Type *llvmType);

protected:
    std::string scopeName;
    std::string name;
    int type;
    llvm::Value *ptr;
};
#endif //VCALCBASE_SYMBOL_H
