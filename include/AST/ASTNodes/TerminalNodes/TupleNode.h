//
// Created by csun on 11/1/18.
//

#ifndef GAZPREABASE_TUPLENODE_H
#define GAZPREABASE_TUPLENODE_H
#include <AST/ASTNodes/BaseNodes/ASTNode.h>
#include <map>

class TupleNode  : public ASTNode{
public:
    explicit TupleNode(std::vector<ASTNode *> *insideElement, int line);
    std::vector<ASTNode*>* getElements();

protected:
    std::vector<ASTNode*>* insideElement;
};


#endif //GAZPREABASE_TUPLENODE_H
