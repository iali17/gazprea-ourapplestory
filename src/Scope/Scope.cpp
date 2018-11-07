//
// Created by kyle on 08/10/18.
//
#include <string>
#include <Scope/Scope.h>
#include <iostream>

Scope::Scope(const std::string &name) : name(name) {
    symbols        = new std::map<std::string, Symbol*>;
    typeSymbols    = new std::map<std::string, GazpreaType*>;
    tupleTypes     = new std::map<llvm::Type*, GazpreaTupleType*>;
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
    typeSymbols = new std::map<std::string, GazpreaType*>;
    tupleTypes  = new std::map<llvm::Type*, GazpreaTupleType*>;
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
        std::cerr << "GazpreaType defined in non-global context\nAborting...\n";
        exit(1);
    }
    typeSymbols->insert(std::pair<std::string, GazpreaType*> (newTypeName, new UserType(newTypeName, newType)));
}

void Scope::addBaseType(std::string newTypeName, llvm::Type *newType) {
    if (enclosingScope != nullptr) {
        std::cerr << "GazpreaType defined in non-global context\nAborting...\n";
        exit(1);
    }
    typeSymbols->insert(std::pair<std::string, GazpreaType*> (newTypeName, new GazpreaType(newTypeName, newType)));
}

GazpreaType *Scope::resolveType(std::string userTypeName) {
    auto iter = typeSymbols->find(userTypeName);
    if(iter == typeSymbols->end())
        return nullptr;
    else
        return iter->second;
}

void Scope::addSymbol(std::string newSymbolName, int type, bool constant) {
    symbols->insert (std::pair<std::string, Symbol*> (newSymbolName, new Symbol(name, newSymbolName, type, constant)));
}

void Scope::addSymbol(std::string newSymbolName, int type, bool constant, llvm::Value *ptr) {
    symbols->insert (std::pair<std::string, Symbol*> (newSymbolName, new Symbol(name, newSymbolName, type, ptr, constant )));
}

void Scope::addFunctionSymbol(std::string newSymbolName, int type, std::vector<ASTNode *> *paramsVec) {
    symbols->insert (std::pair<std::string, Symbol*> (newSymbolName, new FunctionSymbol(name, newSymbolName, type, paramsVec)));
}

//for adding user types
void Scope::addTupleType(std::string newTypeName, llvm::Type *newType, std::unordered_map<std::string, int> *stringRefMap,
                    std::vector<llvm::Type *> *members) {

    GazpreaTupleType *gazpreaTupleType = new GazpreaTupleType(newTypeName, newType, stringRefMap, members);
    typeSymbols->insert(std::pair<std::string, GazpreaType*> (newTypeName, (GazpreaType *) gazpreaTupleType));
}

//for adding tuple types
void Scope::addTupleType(llvm::Type *newType, std::unordered_map<std::string, int> *stringRefMap,
                         std::vector<llvm::Type *> *members) {
    GazpreaTupleType *gazpreaTupleType = new GazpreaTupleType("autogen", newType, stringRefMap, members);
    tupleTypes->insert(std::pair<llvm::Type *, GazpreaTupleType*> (newType, gazpreaTupleType));
}

GazpreaTupleType *Scope::resolveType(llvm::Type *type) {
    llvm::Type * tmp = type;
    auto iter = tupleTypes->find(tmp);
    if(iter == tupleTypes->end())
        return nullptr;
    else
        return iter->second;
}
