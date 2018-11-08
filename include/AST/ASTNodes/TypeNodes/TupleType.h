

#ifndef GAZPREABASE_TUPLETYPE_H
#define GAZPREABASE_TUPLETYPE_H
#include <AST/ASTNodes/BaseNodes/ASTNode.h>

class vector;

class TupleType : public ASTNode {
public:
    explicit TupleType(std::vector<ASTNode *> * decls, int line);
    std::vector<ASTNode *> *getDecls() const;

protected:
    std::vector<ASTNode *> * decls;

};


#endif //GAZPREABASE_TUPLETYPE_H
