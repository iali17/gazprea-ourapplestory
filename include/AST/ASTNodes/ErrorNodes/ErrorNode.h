//
// Created by ali5 on 11/5/18.
//

#ifndef GAZPREABASE_ERRORNODE_H
#define GAZPREABASE_ERRORNODE_H

#include <string>

class ErrorNode {
public:
    enum errorType {SCALAR_ERROR, VECTOR_ERROR, MATRIX_ERROR};

    ErrorNode(errorType etype);
    virtual std::string getNode() = 0;

    errorType getErrorType() const;
private:
    errorType eType;

};

#endif //GAZPREABASE_ERRORNODE_H
