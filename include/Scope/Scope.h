//
// Created by kyle on 08/10/18.
//

#ifndef VCALCBASE_SCOPE_H
#define VCALCBASE_SCOPE_H

#include <unordered_map>
#include "Symbol.h"
#include <map>
#include "UserType.h"

class Scope {
public:
    Scope(const std::string &name);
    Scope(const std::string &name, Scope *enclosingScope);

    std::map<std::string, Symbol*> *getSymbols() const;
    GazpreaType * resolveType(std::string userTypeName);
    Symbol   * resolveSymbol(std::string symbolName);
    Scope    * getEnclosingScope() const;

    void addSymbol(std::string newSymbolName, int type);
    void addSymbol(std::string newSymbolName, int type, bool constant);
    void addSymbol(std::string newSymbolName, int type, bool constant, llvm::Value *ptr);
    void addUserType(std::string newTypeName, llvm::Type* newType);
    void addBaseType(std::string newTypeName, llvm::Type* newType);
protected:
    std::string name;
    std::map<std::string, Symbol*>   *symbols;
    std::map<std::string, GazpreaType*> *typeSymbols;
    Scope * enclosingScope = nullptr;
};
#endif //VCALCBASE_SCOPE_H