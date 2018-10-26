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
