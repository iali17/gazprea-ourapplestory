//
// Created by kyle on 08/10/18.
//
#include "Scope.h"
#include <stack>

#ifndef VCALCBASE_SYMBOLTABLE_H
#define VCALCBASE_SYMBOLTABLE_H

// Pretty much taken from notes and redone for C++
class SymbolTable {
public:
    SymbolTable();
    void pushNewScope(std::string newScopeName);
    void pushNewScope();
    void addSymbol(std::string newSymbolName, int type);
    void addUserType(std::string newTypeName, llvm::Type* newType);
    UserType * resolveUserType(std::string userTypeName);
    Symbol   * resolveSymbol(std::string symbolName);
    void popScope();

protected:
    int scopeCount  = 0;
    std::stack<Scope*> *scopeStack;
    std::string getNewScopeString();
};
#endif //VCALCBASE_SYMBOLTABLE_H
