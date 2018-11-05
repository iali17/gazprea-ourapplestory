//
// Created by ali5 on 11/5/18.
//

#ifndef GAZPREABASE_ERRORBUILDER_H
#define GAZPREABASE_ERRORBUILDER_H

#include "../AST/ASTNodes/ErrorNodes/ErrorNode.h"
#include <iostream>

class ErrorBuilder {
public:
    ErrorBuilder() = default;

    void printError(ErrorNode *error);
};

#endif //GAZPREABASE_ERRORBUILDER_H
