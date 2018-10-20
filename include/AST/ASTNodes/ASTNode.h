//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTNODE_H
#define GAZPREABASE_ASTNODE_H

// Macros for the different types
// TODO - figure out best way to represent node types
#define UNDEF  -1
#define BOOLEAN 0
#define INTEGER 1
#define VECTOR  2

class ASTNode  {
public:
    virtual ~ASTNode() = default;
    int getType() const;
protected:
    int type = UNDEF;
    void setType(int newType);
};
#endif //GAZPREABASE_ASTNODE_H
