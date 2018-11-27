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
    llvm::Type *type;
    if(!values->empty()) type = values->at(0)->getType();
    else type = intTy;
    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));
    et->initVector(vec, it->getConsi32(values->size()));

    vec = it->castVectorToType(vec, type);

    // Populate vector
    if(!values->empty())
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

    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));
    vec = it->castVectorToType(vec, type);
    if (size){
        et->initVector(vec, size);
    }

    if (vectorNode) {
        if (!vectorNode->getElements()->empty()) {
            llvm::Value *vecExpr = visit(node->getExpr());
            llvm::Type *vecType = it->getDeclVectorType(stype);
            llvm::Value *vecExprSize = it->getValFromStruct(vecExpr, it->getConsi32(VEC_LEN_INDEX));

            /*if((vec->getType() == realVecTy->getPointerTo()) && (vecExpr->getType() == intVecTy->getPointerTo()))
                vecExpr = ct->createVecFromVec(vecExpr, realTy, vecExprSize, node->getLine());

            et->strictCopyVectorElements(vec, vecExpr, it->getConsi32(node->getLine()));*/

            vec = ct->typeAssCast(vecType, vecExpr, node->getLine(), size, (int)vectorNode->getElements()->size());
        }
        // empty vector
        else  {
            et->initVector(vec, it->getConsi32(0));
        }


    }
    // Handles case when expr is not a vector -> scalar, haven't tested tuple. Pretty sure segfaults if tested
    else if (node->getExpr()) {

        llvm::Value *regExpr = visit(node->getExpr());
        llvm::Type *vecType = it->getDeclVectorType(stype);

        vec = ct->typeAssCast(vecType, regExpr, node->getLine(), size);
    }

    symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), vec);
    return nullptr;
}

llvm::Value *CodeGenerator::visit(ReverseVectorNode *node) {
    llvm::Value *vecPtr = visit(node->getExpr());
    llvm::Type * ty = it->getVectorElementType(vecPtr);
    vecPtr = et->getReverseVector(vecPtr);
    return it->castVectorToType(vecPtr, ty);
}

llvm::Value *CodeGenerator::visit(LengthNode *node) {
    llvm::Value *vec = visit(node->getExpr());
    return et->getVectorLength(vec);
}