//
// Created by kyle on 05/11/18.
//

#include "AST/AST.h"

ASTNode *IndexTupleNode::getIndex() const {
    return index;
}

IDNode *IndexTupleNode::getIdNode() const {
    return idNode;
}
