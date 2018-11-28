//
// Created by kyle on 08/10/18.
//
#include <string>
#include <Scope/Symbol.h>

Symbol::Symbol(const std::string &scopeName, const std::string &name, int type) : scopeName(scopeName), name(name),
                                                                                  type(type) {
    constant = false;
}

int Symbol::getType() const {
    return type;
}

llvm::Value *Symbol::getPtr() const {
    return ptr;
}

void Symbol::setPtr(llvm::Value *ptr) {
    Symbol::ptr = ptr;
}

const std::string &Symbol::getName() const {
    return name;
}

bool Symbol::isConstant() const {
    return constant;
}

Symbol::Symbol(const std::string &scopeName, const std::string &name, int type, bool constant) : scopeName(scopeName),
                                                                                                 name(name), type(type),
                                                                                                 constant(constant) {}

Symbol::Symbol(const std::string &scopeName, const std::string &name, int type, llvm::Value *ptr, bool constant)
        : scopeName(scopeName), name(name), type(type), ptr(ptr), constant(constant) {}

void Symbol::setConstant(bool constant) {
    Symbol::constant = constant;
}

