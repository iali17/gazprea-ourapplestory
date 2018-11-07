//
// Created by ali5 on 11/5/18.
//

#include <InternalTools/ErrorBuilder.h>

void ErrorBuilder::printError(ErrorNode *error) {
    std::cerr << error->getNode();
    exit(1);
}