//
// Created by iali1 on 11/6/18.
//

#ifndef GAZPREABASE_TUPLEMEMBERASSNODE_H
#define GAZPREABASE_TUPLEMEMBERASSNODE_H

#include <AST/ASTNodes/BaseNodes/UnaryNode.h>
#include <AST/ASTNodes/TerminalNodes/IndexTupleNode.h>

class TupleMemberAssNode : public UnaryNode {
public:
    TupleMemberAssNode(ASTNode * expr, IndexTupleNode * LHS);
    IndexTupleNode *getLHS() const;

protected:
    IndexTupleNode * LHS;
};


#endif //GAZPREABASE_TUPLEMEMBERASSNODE_H
