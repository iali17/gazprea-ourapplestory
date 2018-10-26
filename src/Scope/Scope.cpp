//
// Created by kyle on 08/10/18.
//

#include <string>
#include <Scope/Scope.h>
#include <iostream>

Scope::Scope(const std::string &name) : name(name) {
    symbols        = new std::map<std::string, Symbol*>;
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
    symbols = new std::map<std::string, Symbol*>;
}

Symbol *Scope::resolveSymbol(std::string symbolName) {
    auto iter = symbols->find(symbolName);
    if(iter == symbols->end())
        return nullptr;
    else
        return iter->second;
}


