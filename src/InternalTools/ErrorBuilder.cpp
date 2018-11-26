//
// Created by ali5 on 11/5/18.
//

#include <InternalTools/ErrorBuilder.h>

void ErrorBuilder::printError(ErrorNode *error) {
    if (error->getErrorType() == ErrorNode::VECTOR_ERROR) {
        if (((VectorErrorNode *) error)->isStv()) {
            std::cerr << ((VectorErrorNode *) error)->getSTVNode();
            exit(1);
        } else {
            std::cerr << error->getNode();
            exit(1);
        }
    } else {
        std::cerr << error->getNode();
        exit(1);
    }
}