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

    return vec;
}


llvm::Value *CodeGenerator::visit(VectorDeclNode *node) {
    VectorNode *vectorNode = nullptr;
    auto *vectorType = dynamic_cast<VectorType *>(node->getVectorType());

    vectorNode = dynamic_cast<VectorNode *>(node->getExpr());
    std::string stype = vectorType->getStringType();
    llvm::Type *type = it->getVectorType(stype);
    llvm::Value *size = visit(node->getSize());
    /*
     * ASTNode *vnode = dynamic_cast<VectorCastNode *>(node)->getVector();
    std::string stype = dynamic_cast<VectorType *>(vnode)->getStringType();
    type = it->getVectorType(stype);

    // Extension size if exists
    llvm::Value *size  = visit(dynamic_cast<VectorType *>(vnode)->getSize());

    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));
    vec = it->castVectorToType(vec, type);

     */

    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));
    vec = it->castVectorToType(vec, type);
    if (size){
        et->initVector(vec, size);
    }

    if (vectorNode) {
        //TODO: this

        llvm::Value *vecExpr = visit(node->getExpr());
        llvm::Type *vecType = it->getDeclVectorType(stype);

        vec = ct->typeAssCast(vecType, vecExpr, node->getLine(), size);

    }

//    if(node->getStringType() == "integervector") {
//        ptr = et->getNewVector(it->getConsi32(INTEGER));
//    } else if(node->getStringType() == "charactervector") {
//        ptr = et->getNewVector(it->getConsi32(CHAR));
//    } else if(node->getStringType() == "booleanvector") {
//        ptr = et->getNewVector(it->getConsi32(BOOLEAN));
//    } else if(node->getStringType() == "realvector") {
//        ptr = et->getNewVector(it->getConsi32(REAL));
//    }

//    llvm::Value *length = visit(node->getVectorType());
//    et->initVector(ptr, length);

    symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), vec);
    return nullptr;
}


llvm::Value *CodeGenerator::visit(ReverseVectorNode *node) {
    llvm::Value *vecPtr = visit(node->getExpr());
    llvm::Type * ty = it->getVectorElementType(vecPtr);
    vecPtr = et->getReverseVector(vecPtr);
    return it->castVectorToType(vecPtr, ty);
}

