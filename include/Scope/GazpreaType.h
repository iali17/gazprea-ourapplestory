//
// Created by kyle on 27/10/18.
//

#ifndef GAZPREABASE_TYPE_H
#define GAZPREABASE_TYPE_H
#include <llvm/IR/Type.h>

class GazpreaType {
public:
    GazpreaType(const std::string &name, llvm::Type *typeDef, int dim1 = -1, int dim2 = -1);
    const std::string &getName() const;
    llvm::Type *getTypeDef() const;
    int getDim1() const;
    void setDim1(int dim1);
    int getDim2() const;
    void setDim2(int dim2);

protected:
    std::string name;
    llvm::Type* typeDef;
    int dim1;
    int dim2;
};


#endif //GAZPREABASE_TYPE_H
