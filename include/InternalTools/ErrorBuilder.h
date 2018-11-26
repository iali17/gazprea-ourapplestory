//
// Created by ali5 on 11/5/18.
//

#ifndef GAZPREABASE_ERRORBUILDER_H
#define GAZPREABASE_ERRORBUILDER_H
#include "AST/ASTNodes/ErrorNodes/ErrorNode.h"
#include "AST/ASTNodes/ErrorNodes/ScalarErrorNode.h"
#include "AST/ASTNodes/ErrorNodes/VectorErrorNode.h"
#include <iostream>

// This class takes an ErrorNode and prints the error associated with it
class ErrorBuilder {
public:
    ErrorBuilder() = default;

    void printError(ErrorNode *error);
};

#endif //GAZPREABASE_ERRORBUILDER_H
