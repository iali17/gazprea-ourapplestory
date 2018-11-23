//
// Created by csun on 11/22/18.
//

#ifndef GAZPREABASE_INTERVALDECL_H
#define GAZPREABASE_INTERVALDECL_H


#include <AST/ASTNodes/BaseNodes/UnaryNode.h>

class IntervalDeclNode : public UnaryNode {
public:
    IntervalDeclNode(ASTNode *expr, bool constant, const std::string &ID, int line);

    bool isConstant() const;
    const std::string &getID() const;
protected:
    bool constant;
    std::string ID;
};


#endif //GAZPREABASE_INTERVALDECL_H
