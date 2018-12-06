//
// Created by kyle on 08/10/18.
//
#include "Scope.h"
#include <stack>
#include <unordered_set>

#ifndef VCALCBASE_SYMBOLTABLE_H
#define VCALCBASE_SYMBOLTABLE_H
#include "AST/AST.h"


// Pretty much taken from notes and redone for C++
class SymbolTable {
public:
    SymbolTable();
    GazpreaType      *resolveType(std::string userTypeName);
    GazpreaTupleType *resolveTupleType(llvm::Type * tupleType);
    Symbol           *resolveSymbol(std::string symbolName);
    std::vector<llvm::Value *> *getAllFreeableVariables();
    void pushNewScope(std::string newScopeName);
    void pushNewScope();
    void addSymbol(std::string newSymbolName, int type);
    void addSymbol(std::string newSymbolName, int type, bool isConstant = false);
    void addSymbol(std::string newSymbolName, int type, bool isConstant, llvm::Value *ptr);
    void addFunctionSymbol(std::string newSymbolName, int type, std::vector<ASTNode *> *paramsVec);
    void addTupleType(std::string newTypeName, llvm::Type *newType, std::unordered_map<std::string, int> *stringRefMap,
                      std::vector<llvm::Type *> *members);
    void addTupleType(llvm::Type *newType, std::unordered_map<std::string, int> *stringRefMap,
                      std::vector<llvm::Type *> *members);
    void addUserType(std::string newTypeName, llvm::Type* newType, int leftSize = -1, int rightSize = -1);
    void addBaseType(std::string baseTypeName, llvm::Type *newType);
    void popScope();

protected:
    int scopeCount  = 0;
    std::stack<Scope*> *scopeStack;
    std::string getNewScopeString();
    std::vector<Scope *> * allScopes;
};
#endif //VCALCBASE_SYMBOLTABLE_H
