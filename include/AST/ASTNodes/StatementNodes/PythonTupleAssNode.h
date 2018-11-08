//
// Created by csun on 11/5/18.
//

#ifndef GAZPREABASE_PYTHONTUPLEASSNODE_H
#define GAZPREABASE_PYTHONTUPLEASSNODE_H
#include <AST/ASTNodes/BaseNodes/ASTNode.h>
#include <AST/ASTNodes/BaseNodes/UnaryNode.h>

class PythonTupleAssNode : public UnaryNode {
public:
    PythonTupleAssNode(ASTNode *expr, const std::vector<std::string> &IDs, int line);
    const std::vector<std::string> &getIDs() const;

protected:
    std::vector<std::string> IDs;
};


#endif //GAZPREABASE_PYTHONTUPLEASSNODE_H
