//
// Created by kyle on 08/10/18.
//

#include <string>
#include <Scope/Symbol.h>

Symbol::Symbol(const std::string &scopeName, const std::string &name, int type) : scopeName(scopeName), name(name),
                                                                                  type(type) {}

int Symbol::getType() const {
    return type;
}

llvm::Value *Symbol::getPtr() const {
    return ptr;
}

void Symbol::setPtr(llvm::Value *ptr) {
    Symbol::ptr = ptr;
}

llvm::Type *Symbol::getLLVMType() const {
    return llvmType;
}

void Symbol::setLLVMType(llvm::Type *llvmType) {
    Symbol::llvmType = llvmType;
}
