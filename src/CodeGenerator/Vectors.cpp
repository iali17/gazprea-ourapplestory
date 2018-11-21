//
// Created by kyle on 20/11/18.
//

#include <CodeGenerator/CodeGenerator.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;
extern llvm::Type *vecTy;
extern llvm::Type *matrixTy;
extern llvm::Type *intVecTy;
extern llvm::Type *intMatrixTy;
extern llvm::Type *charVecTy;
extern llvm::Type *charMatrixTy;
extern llvm::Type *boolVecTy;
extern llvm::Type *boolMatrixTy;
extern llvm::Type *realVecTy;
extern llvm::Type *realMatrixTy;
extern llvm::Type *intervalTy;

llvm::Value *CodeGenerator::visit(VectorNode *node) {
    auto values = new std::vector<llvm::Value *>();

    for(uint i = 0; i < node->getElements()->size(); i++)
        values->push_back(visit(node->getElements()->at(i)));

    // Casting empty vector with proper type
    llvm::Type *type = values->at(0)->getType();
    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));
    et->initVector(vec, it->getConsi32(values->size()));

    vec = it->castVectorToType(vec, type);

    // Populate vector
    it->setVectorValues(vec, values);
    et->printVector(vec);

    return vec;
}