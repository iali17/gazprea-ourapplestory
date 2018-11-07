//
// Created by kyle on 08/10/18.
//

#include <stack>
#include <Scope/SymbolTable.h>
#include <iostream>

SymbolTable::SymbolTable() {
    scopeStack = new std::stack<Scope*>;
}

void SymbolTable::pushNewScope(std::string newScopeName) {
    if(scopeStack->empty())
        scopeStack->push(new Scope(newScopeName));
    else
        scopeStack->push(new Scope(newScopeName, scopeStack->top()));
}

void SymbolTable::addSymbol(std::string newSymbolName, int type) {
    scopeStack->top()->addSymbol(newSymbolName, type);
}

void SymbolTable::addSymbol(std::string newSymbolName, int type, bool isConstant) {
    scopeStack->top()->addSymbol(newSymbolName, type, isConstant);
}

void SymbolTable::popScope() {
    scopeStack->pop();
}

Symbol *SymbolTable::resolveSymbol(std::string symbolName) {
    Scope  *scope = scopeStack->top();
    Symbol * symbol = nullptr;

    while(scope != nullptr) {
        symbol = scope->resolveSymbol(symbolName);

        if(symbol != nullptr)
            break;
        scope = scope->getEnclosingScope();
    }
    return symbol;
}

std::string SymbolTable::getNewScopeString(){
    return "_s" + std::to_string(scopeCount++) + "_";
}

void SymbolTable::pushNewScope() {
    pushNewScope(getNewScopeString());
}

void SymbolTable::addUserType(std::string newTypeName, llvm::Type *newType) {
    scopeStack->top()->addUserType(newTypeName, newType);
}

void SymbolTable::addBaseType(std::string baseTypeName, llvm::Type *newType) {
    scopeStack->top()->addBaseType(baseTypeName, newType);
}

GazpreaType *SymbolTable::resolveType(std::string userTypeName) {
    Scope  *scope = scopeStack->top();
    GazpreaType * userType = nullptr;

    while(scope != nullptr) {
        userType = scope->resolveType(userTypeName);

        if(userType != nullptr)
            break;
        scope = scope->getEnclosingScope();
    }
    return userType;
}

void SymbolTable::addSymbol(std::string newSymbolName, int type, bool isConstant, llvm::Value *ptr) {
    scopeStack->top()->addSymbol(newSymbolName, type, isConstant, ptr);
}

void
SymbolTable::addFunctionSymbol(std::string newSymbolName, int type, std::vector<ASTNode *> *paramsVec) {
    scopeStack->top()->addFunctionSymbol(newSymbolName, type, paramsVec);
}

void SymbolTable::addTupleType(std::string newTypeName, llvm::Type *newType,
                               std::unordered_map<std::string, int> *stringRefMap, std::vector<llvm::Type *> *members) {
    scopeStack->top()->addTupleType(newTypeName, newType, stringRefMap, members);
}

void SymbolTable::addTupleType(llvm::Type *newType, std::unordered_map<std::string, int> *stringRefMap,
                               std::vector<llvm::Type *> *members) {
    scopeStack->top()->addTupleType(newType, stringRefMap, members);
}

GazpreaTupleType *SymbolTable::resolveTupleType(llvm::Type *tupleType) {
    Scope  *scope = scopeStack->top();
    GazpreaTupleType *type = nullptr;

    while(scope != nullptr) {
        type = scope->resolveType(tupleType);

        if(type != nullptr)
            break;
        scope = scope->getEnclosingScope();
    }
    return type;
}
