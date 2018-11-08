//
// Created by kyle on 08/11/18.
//

#ifndef GAZPREABASE_TUPLEINPUTNODE_H
#define GAZPREABASE_TUPLEINPUTNODE_H
#include "AST/ASTNodes/BaseNodes/IONode.h"
#include "AST/ASTNodes/TerminalNodes/IndexTupleNode.h"

class TupleInputNode : public IONode {
public:
    TupleInputNode(const std::string &streamName, ASTNode *indexNode, int line);
    IndexTupleNode *getIndexTupleNode() const;

protected:
    IndexTupleNode * indexTupleNode;
};
#endif //GAZPREABASE_TUPLEINPUTNODE_H
