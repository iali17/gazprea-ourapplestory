//
// Created by ali5 on 11/20/18.
//

#ifndef GAZPREABASE_VECTORNODE_H
#define GAZPREABASE_VECTORNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

class VectorNode : public ASTNode {
public:
    explicit VectorNode(std::vector<ASTNode *> *elements, int line);

    std::vector<ASTNode *>* getElements();

protected:
    std::vector<ASTNode *> *elements;
};

#endif //GAZPREABASE_VECTORNODE_H
