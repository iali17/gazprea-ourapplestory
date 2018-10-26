//
// Created by kyle on 08/10/18.
//

#include <unordered_map>
#include "Symbol.h"
#include <map>

#ifndef VCALCBASE_SCOPE_H
#define VCALCBASE_SCOPE_H
class Scope {
public:
    Scope(const std::string &name);
    Scope(const std::string &name, Scope *enclosingScope);

    std::map<std::string, Symbol*> *getSymbols() const;
    void addSymbol(std::string newSymbolName, int type);
    Symbol * resolveSymbol(std::string symbolName);

protected:
    std::string name;
    std::map<std::string, Symbol*> *symbols;
    Scope * enclosingScope = nullptr;
public:
    Scope *getEnclosingScope() const;
};
#endif //VCALCBASE_SCOPE_H
