//
// Created by csun on 11/1/18.
//

#ifndef GAZPREABASE_TUPLENODE_H
#define GAZPREABASE_TUPLENODE_H


#include <AST/ASTNodes/BaseNodes/ASTNode.h>
#include <map>

class TupleNode  : public ASTNode{
public:
    TupleNode(std::vector<ASTNode *> *insideElement);

    std::vector<ASTNode*>* insideElement;



};


#endif //GAZPREABASE_TUPLENODE_H
