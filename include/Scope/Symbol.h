//
// Created by kyle on 08/10/18.
//

#ifndef VCALCBASE_SYMBOL_H
#define VCALCBASE_SYMBOL_H

#include <string>
#include <llvm/IR/Value.h>
#include "AST/AST.h"
// Pretty much taken from the notes and refactored for C++
class Symbol {
public:
    Symbol(const std::string &scopeName, const std::string &name, int type);
    llvm::Value *getPtr() const;
    void setPtr(llvm::Value *ptr);
    int getType() const;
    const std::string &getName() const;
    bool isConstant() const;
    Symbol(const std::string &scopeName, const std::string &name, int type, bool constant);

    Symbol(const std::string &scopeName, const std::string &name, int type, llvm::Value *ptr, bool constant);

protected:
    std::string scopeName;
    std::string name;
    int type;
    llvm::Value *ptr;
public:
    void setConstant(bool constant);

protected:
    bool constant;
};
#endif //VCALCBASE_SYMBOL_H