//
// Created by kyle on 26/10/18.
//

#ifndef GAZPREABASE_DOLOOPNODE_H
#define GAZPREABASE_DOLOOPNODE_H

#include <AST/ASTNodes/BaseNodes/ASTNode.h>
#include <AST/ASTNodes/BaseNodes/BaseLoopNode.h>

class DoLoopNode : public BaseLoopNode {
public:
    DoLoopNode(ASTNode *body, ASTNode *control, int line);

    int getLine();

private:
    int line;
};
#endif //GAZPREABASE_DOLOOPNODE_H
