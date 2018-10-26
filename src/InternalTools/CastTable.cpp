//
// Created by ali5 on 10/26/18.
//

#include <InternalTools/CastTable.h>

#include "InternalTools/CastTable.h"

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;

int CastTable::getType(llvm::Type *expr) {
    if(expr == i64Ty || expr == i32Ty || expr == i8Ty || expr == intTy)
        return 2;
    else if(expr == realTy)
        return 3;
    else if(expr == charTy)
        return 1;
    else if(expr == boolTy)
        return 0;
}

void CastTable::typeCast(llvm::Value *leftExpr, llvm::Value *rightExpr) {
    llvm::Value *lValueLoad = ir->CreateLoad(leftExpr);
    llvm::Value *rValueLoad = ir->CreateLoad(rightExpr);

    // Type of left and right expr
    llvm::Type *lTypeP = lValueLoad->getType();
    llvm::Type *rTypeP = rValueLoad->getType();

    // Position in typeTable
    int lType = getType(lTypeP);
    int rType = getType(rTypeP);

    // Cast type
    std::string castType = typeTable[lType][rType];
    std::string lTypeString = typeTable[lType][lType];
    std::string rTypeString = typeTable[rType][rType];

    // Type cast to bool
    if(castType == "bool") {
        if(lTypeString == "int" || rTypeString == "int") {
            if(lTypeString == "int")
                ir->CreateBitCast(lValueLoad, boolTy);
            else
                ir->CreateBitCast(rValueLoad, boolTy);
        }
        else if(lTypeString == "char" || rTypeString == "char") {
            if(lTypeString == "char")
                ir->CreateBitCast(lValueLoad, boolTy);
            else
                ir->CreateBitCast(rValueLoad, boolTy);
        }
    }

    // Type cast to char
    else if(castType == "char") {
        if(lTypeString == "bool" || rTypeString == "bool") {


        }
    }
}

void CastTable::upCast(llvm::Value *type, llvm::Value *expr) {
}
