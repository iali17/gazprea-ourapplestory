//
// Created by andyli on 21/11/18.
//

#ifndef GAZPREABASE_SCALARCASTNODE_H
#define GAZPREABASE_SCALARCASTNODE_H

#include "CastExprNode.h"

class ScalarCastNode : public CastExprNode {
public:
    ScalarCastNode(ASTNode *expr, const std::string &type, int line);

    const std::string &getTypeString() const;

protected:
    std::string type;

};

#endif //GAZPREABASE_SCALARCASTNODE_H
