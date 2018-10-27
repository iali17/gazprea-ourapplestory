//
// Created by kyle on 22/10/18.
//

#ifndef GAZPREABASE_PARAMNODE_H
#define GAZPREABASE_PARAMNODE_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"

/**
 * This class is basically empty TODO - everything
 */
class ParamNode : public ASTNode {
public:
    ParamNode(const std::string &declaredType, const std::string &varName);

protected:
public:
    const std::string &getDeclaredType() const;

    const std::string &getVarName() const;

protected:
    std::string declaredType;
    std::string varName;
};

#endif //GAZPREABASE_PARAMNODE_H
