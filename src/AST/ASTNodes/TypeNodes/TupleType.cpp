//
// Created by csun on 11/4/18.
//

#include <AST/ASTNodes/TypeNodes/TupleType.h>


TupleType::TupleType(std::vector<ASTNode *> *decls) : decls(decls){

}

std::vector<ASTNode *> *TupleType::getDecls() const {
    return decls;
}

