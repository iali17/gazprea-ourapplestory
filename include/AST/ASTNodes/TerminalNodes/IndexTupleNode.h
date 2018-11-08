//
// Created by kyle on 05/11/18.
//

#ifndef GAZPREABASE_INDEXTUPLENODE_H
#define GAZPREABASE_INDEXTUPLENODE_H
#include "AST/ASTNodes/TerminalNodes/IDNode.h"

class IndexTupleNode : public ASTNode {
public:
    IndexTupleNode(ASTNode *index, IDNode *idNode, int line);
    ASTNode *getIndex() const;
    IDNode *getIdNode() const;

protected:
    ASTNode *index;
    IDNode  *idNode;
};
#endif //GAZPREABASE_INDEXTUPLENODE_H
