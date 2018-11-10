//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTNODE_H
#define GAZPREABASE_ASTNODE_H

// Macros for the different types
// TODO - figure out best way to represent node types
#include <llvm/IR/Type.h>

#define IDENTITY -3
#define NULLTY   -2
#define UNDEF    -1
#define INSTREAM  0
#define OUTSTREAM 1
#define BOOLEAN   2
#define CHAR      3
#define INTEGER   4
#define REAL      5
#define TUPLE     6
#define VECTOR    7
#define MATRIX    8
#define NEQ 0
#define EQ  1


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
