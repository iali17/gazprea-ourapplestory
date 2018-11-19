//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTNODE_H
#define GAZPREABASE_ASTNODE_H

// Macros for the different types
// TODO - figure out best way to represent node types
#include <llvm/IR/Type.h>
#include "globals.h"


class ASTNode  {
public:
    virtual ~ASTNode() = default;
    explicit ASTNode(int);
    virtual int getType() const;
    llvm::Type *getLlvmType() const;
    void setLlvmType(llvm::Type *llvmType);
    int getLine() const;
    void setLine(int);

protected:
    int type = UNDEF;
    llvm::Type * llvmType = nullptr;
    void setType(int newType);
    int line;
};


#endif //GAZPREABASE_ASTNODE_H
