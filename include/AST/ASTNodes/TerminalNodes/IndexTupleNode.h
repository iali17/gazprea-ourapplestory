//
// Created by kyle on 05/11/18.
//

#ifndef GAZPREABASE_INDEXTUPLENODE_H
#define GAZPREABASE_INDEXTUPLENODE_H
#include "AST/ASTNodes/TerminalNodes/IDNode.h"

class IndexTupleNode : public ASTNode {
protected:
    ASTNode *index;
    IDNode  *idNode;
public:
    ASTNode *getIndex() const;
    IDNode *getIdNode() const;
    IndexTupleNode(IDNode *idNode, ASTNode *index) :index(index), idNode(idNode) {}
};
#endif //GAZPREABASE_INDEXTUPLENODE_H
