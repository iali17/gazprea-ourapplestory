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

llvm::Value *CastTable::typeCast(llvm::Value *leftExpr, llvm::Value *rightExpr) {
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
        // Cast from int to bool
        if(lTypeString == "int" || rTypeString == "int") {
            uint64_t trueValue = static_cast<uint64_t>(static_cast<int64_t>(0));
            llvm::Value *zero = llvm::ConstantInt::get(i32Ty, trueValue);

            if(lTypeString == "int") {
                ir->CreateICmpNE(lValueLoad, zero, "cast");
            }
            else {
                ir->CreateICmpNE(rValueLoad, zero, "cast");
            }
        }
        // Cast from char to bool
        else if(lTypeString == "char" || rTypeString == "char") {
            if(lTypeString == "char") {

            }
            else {

            }
        }
    }

    // Type cast to char
    else if(castType == "char") {
        // Cast from bool to char
        if(lTypeString == "bool" || rTypeString == "bool") {
            if(lTypeString == "bool") {

            }
            else {

            }
        }
        // Cast from int to char
        else if(lTypeString == "int" || rTypeString == "int") {
            if(lTypeString == "int") {

            }
            else {

            }
        }
    }

    // Type cast to int
    else if(castType == "int") {
        // Cast from bool to int
        if(lTypeString == "bool" || rTypeString == "bool") {
            if(lTypeString == "bool") {

            }
            else {

            }
        }
        // Cast from char to int
        else if(lTypeString == "char" || rTypeString == "char") {
            if(lTypeString == "char") {

            }
            else {

            }
        }
        // Cast from float to int
        else if(lTypeString == "float" || rTypeString == "float") {
            if(lTypeString == "float") {

            }
            else {

            }
        }
    }

    // Type cast to float
    else if(castType == "float") {
        // Cast from bool to float
        if(lTypeString == "bool" || rTypeString == "bool") {
            if(lTypeString == "bool") {

            }
            else {

            }
        }
        // Cast from char to float
        else if(lTypeString == "char" || rTypeString == "char") {
            if(lTypeString == "char") {

            }
            else {

            }
        }
        // Cast from int to float
        else if(lTypeString == "int" || rTypeString == "int") {
            if(lTypeString == "int") {

            }
            else {

            }
        }
    }

    // Invalid cast, return error
    else {

    }
}

llvm::Value *CastTable::upCast(llvm::Type *type, llvm::Value *expr) {
    llvm::Value *exprLoad = ir->CreateLoad(expr);

    // Type of expr
    llvm::Type *exprType = exprLoad->getType();

    // Position in table
    int exprPos = getType(exprType);
    int typePos = getType(type);

    // Cast type
    std::string exprString = typeTable[exprPos][exprPos];
    std::string typeString = typeTable[typePos][typePos];

    // Casting expr to bool
    if(typeString == "bool"){

    }

    // Casting expr to char
    else if(typeString == "char") {

    }

    // Casting expr to int
    else if(typeString == "int") {

    }

    // Casting expr to float
    else if(typeString == "float") {

    }

    // Can't cast to type, return an error
    else {

    }
}
