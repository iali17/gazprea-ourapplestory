

#ifndef GAZPREABASE_TUPLETYPE_H
#define GAZPREABASE_TUPLETYPE_H
#include <AST/ASTNodes/BaseNodes/ASTNode.h>

class TupleTypeNode : public ASTNode {
public:
    explicit TupleTypeNode(std::vector<ASTNode *> * decls, int line);
    std::vector<ASTNode *> *getDecls() const;

protected:
    std::vector<ASTNode *> * decls;

};


#endif //GAZPREABASE_TUPLETYPE_H
