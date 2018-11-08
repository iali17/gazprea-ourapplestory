//
// Created by lepoidev on 10/26/18.
//

#ifndef GAZPREABASE_CHARNODE_H
#define GAZPREABASE_CHARNODE_H
#include <AST/ASTNodes/BaseNodes/ASTNode.h>

class CharNode : public ASTNode {
public:
    explicit CharNode(char val, int line);
    char getVal() const;

protected:
    char val;
};
#endif //GAZPREABASE_CHARNODE_H
