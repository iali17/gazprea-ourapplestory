//
// Created by kyle on 08/10/18.
//

#ifndef VCALCBASE_SCOPE_H
#define VCALCBASE_SCOPE_H

#include <unordered_map>
#include "Symbol.h"
#include <map>
#include "UserType.h"
#include "FunctionSymbol.h"
#include "GazpreaTupleType.h"
#include "llvm/IR/DerivedTypes.h"

class Scope {
public:
    Scope(const std::string &name);
    Scope(const std::string &name, Scope *enclosingScope);

    std::map<std::string, Symbol*> *getSymbols() const;
    GazpreaType      * resolveType(std::string userTypeName);
    GazpreaTupleType * resolveType(llvm::Type * type);
    Symbol           * resolveSymbol(std::string symbolName);
    Scope            * getEnclosingScope() const;

    std::vector<llvm::Value *> * getFreeableVariables();
    void addSymbol(std::string newSymbolName, int type);
    void addSymbol(std::string newSymbolName, int type, bool constant);
    void addSymbol(std::string newSymbolName, int type, bool constant, llvm::Value *ptr);
    void addUserType(std::string newTypeName, llvm::Type* newType, int leftSize = -1, int rightSize = -1);
    void addBaseType(std::string newTypeName, llvm::Type* newType);
    void addTupleType(std::string newTypeName, llvm::Type *newType, std::unordered_map<std::string, int> *stringRefMap,
                      std::vector<llvm::Type *> *members);
    void addTupleType(llvm::Type *newType, std::unordered_map<std::string, int> *stringRefMap,
                      std::vector<llvm::Type *> *members);
    void addFunctionSymbol(std::string newSymbolName, int type, std::vector<ASTNode *> *paramsVec);
protected:
    std::string name;
    std::map<std::string, Symbol*>   *symbols;
    std::map<std::string, GazpreaType*> *typeSymbols;
    std::map<llvm::Type *, GazpreaTupleType*> *tupleTypes;
    Scope * enclosingScope = nullptr;
};
#endif //VCALCBASE_SCOPE_H