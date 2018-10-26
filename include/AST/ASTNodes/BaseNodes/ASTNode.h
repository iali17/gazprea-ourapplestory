//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTNODE_H
#define GAZPREABASE_ASTNODE_H

// Macros for the different types
// TODO - figure out best way to represent node types
#include <llvm/IR/Type.h>

#define NULLTY -2
#define UNDEF  -1
#define BOOLEAN 0
#define INTEGER 1
#define VECTOR  2
#define REAL    3
#define CHAR    4

class ASTNode  {
public:
    virtual ~ASTNode() = default;
    int getType() const;

    llvm::Type *getLlvmType() const;

    void setLlvmType(llvm::Type *llvmType);

protected:
    int type = UNDEF;
    llvm::Type * llvmType;
    void setType(int newType);
};


#endif //GAZPREABASE_ASTNODE_H
