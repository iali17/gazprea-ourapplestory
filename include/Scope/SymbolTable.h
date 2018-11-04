//
// Created by kyle on 08/10/18.
//
#include "Scope.h"
#include <stack>

#ifndef VCALCBASE_SYMBOLTABLE_H
#define VCALCBASE_SYMBOLTABLE_H

#include "AST/AST.h"
// Pretty much taken from notes and redone for C++
class SymbolTable {
public:
    SymbolTable();
    GazpreaType * resolveType(std::string userTypeName);
    Symbol      * resolveSymbol(std::string symbolName);
    void pushNewScope(std::string newScopeName);
    void pushNewScope();
    void addSymbol(std::string newSymbolName, int type);
    void addSymbol(std::string newSymbolName, int type, bool isConstant = false);
    void addSymbol(std::string newSymbolName, int type, bool isConstant, llvm::Value *ptr);
    void addFunctionSymbol(std::string newSymbolName, int type, std::vector<ASTNode *> *paramsVec);
    void addUserType(std::string newTypeName, llvm::Type* newType);
    void addBaseType(std::string baseTypeName, llvm::Type *newType);
    void popScope();

protected:
    int scopeCount  = 0;
    std::stack<Scope*> *scopeStack;
    std::string getNewScopeString();
};
#endif //VCALCBASE_SYMBOLTABLE_H
