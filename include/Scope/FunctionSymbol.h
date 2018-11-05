//
// Created by lepoidev on 11/4/18.
//

#ifndef GAZPREABASE_FUNCTIONTYPE_H
#define GAZPREABASE_FUNCTIONTYPE_H
#include <vector>
#include "Symbol.h"

class FunctionSymbol : public Symbol {
public:
    std::vector<ASTNode *> *getParamsVec() const;

protected:
    std::vector<ASTNode *> *paramsVec;
public:
    FunctionSymbol(const std::string &scopeName, const std::string &name, int type, std::vector<ASTNode *> *paramsVec);
};
#endif //GAZPREABASE_FUNCTIONTYPE_H
