//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTNODE_H
#define GAZPREABASE_ASTNODE_H

// Macros for the different types
// TODO - figure out best way to represent node types
#include <llvm/IR/Type.h>

#define NULLTY   -2
#define UNDEF    -1
#define INSTREAM  0
#define OUTSTREAM 1
#define BOOLEAN   2
#define INTEGER   3
#define VECTOR    4
#define REAL      5
#define CHAR      6
#define TUPLE     7

class ASTNode  {
public:
    virtual ~ASTNode() = default;

    virtual int getType() const;

    llvm::Type *getLlvmType() const;

    void setLlvmType(llvm::Type *llvmType);

protected:
    int type = UNDEF;
    llvm::Type * llvmType;
    void setType(int newType);
};


#endif //GAZPREABASE_ASTNODE_H
