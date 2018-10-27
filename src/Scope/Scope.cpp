//
// Created by kyle on 08/10/18.
//

#include <string>
#include <Scope/Scope.h>
#include <iostream>

Scope::Scope(const std::string &name) : name(name) {
    symbols        = new std::map<std::string, Symbol*>;
    typeSymbols    = new std::map<std::string, UserType*>;
    enclosingScope = nullptr;
}

void Scope::addSymbol(std::string newSymbolName, int type) {
    symbols->insert (std::pair<std::string, Symbol*> (newSymbolName, new Symbol(name, newSymbolName, type)));
}

std::map<std::string, Symbol*> *Scope::getSymbols() const {
    return symbols;
}

Scope *Scope::getEnclosingScope() const {
    return enclosingScope;
}

Scope::Scope(const std::string &name,  Scope *enclosingScope) : name(name), enclosingScope(enclosingScope) {
    symbols     = new std::map<std::string, Symbol*>;
    typeSymbols = new std::map<std::string, UserType*>;
}

Symbol *Scope::resolveSymbol(std::string symbolName) {
    auto iter = symbols->find(symbolName);
    if(iter == symbols->end())
        return nullptr;
    else
        return iter->second;
}

void Scope::addUserType(std::string newTypeName, llvm::Type* newType) {
    if (enclosingScope != nullptr) {
        std::cerr << "Type defined in non-global context\nAborting...\n";
        exit(1);
    }
    typeSymbols->insert(std::pair<std::string, UserType*> (newTypeName, new UserType(newTypeName, newType)));
}

UserType *Scope::resolveUserType(std::string userTypeName) {
    auto iter = typeSymbols->find(userTypeName);
    if(iter == typeSymbols->end())
        return nullptr;
    else
        return iter->second;
}


