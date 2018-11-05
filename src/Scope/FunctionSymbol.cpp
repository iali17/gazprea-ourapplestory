//
// Created by lepoidev on 11/4/18.
//

#include "Scope/FunctionSymbol.h"

FunctionSymbol::FunctionSymbol(const std::string &scopeName, const std::string &name, int type,
                               std::vector<ASTNode *> *paramsVec) : Symbol(scopeName, name, type),
                                                                    paramsVec(paramsVec) {}

std::vector<ASTNode *> *FunctionSymbol::getParamsVec() const {
    return paramsVec;
}
