//
// Created by csun on 11/1/18.
//

#ifndef GAZPREABASE_TUPLEDECLNODE_H
#define GAZPREABASE_TUPLEDECLNODE_H
#include <AST/ASTNodes/BaseNodes/UnaryNode.h>

class TupleDeclNode : public UnaryNode {
public:
    TupleDeclNode(ASTNode *expr, bool constant, const std::string &ID, ASTNode *tupleTypes);
    const std::string &getID() const;
    bool isConstant() const;
    ASTNode * getTupleTypes() const;

protected:
    bool constant;
    std::string ID;
    ASTNode * tupleTypes; // represent each empty decl inside the tuple()

};


#endif //GAZPREABASE_TUPLEDECLNODE_H
