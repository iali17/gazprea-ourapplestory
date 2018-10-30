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

CastTable::CastTable(llvm::LLVMContext *globalctx, llvm::IRBuilder<> *ir) : globalCtx(globalctx), ir(ir) {
    // Do nothing
}

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

    // Gazprea type of left and right expr
    llvm::Type *lTypeP = lValueLoad->getType();
    llvm::Type *rTypeP = rValueLoad->getType();

    // Position in typeTable
    int lType = getType(lTypeP);
    int rType = getType(rTypeP);

    // Cast type
    std::string castType = typeTable[lType][rType];
    std::string lTypeString = typeTable[lType][lType];
    std::string rTypeString = typeTable[rType][rType];

    // GazpreaType cast to bool
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

    // GazpreaType cast to char
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

    // GazpreaType cast to int
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

    // GazpreaType cast to float
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
                ir->CreateSIToFP(lValueLoad, realTy);

                std::cout << "left upcast\n";
            }
            else {
                ir->CreateSIToFP(rValueLoad, realTy);

                std::cout << "right upcast\n";
            }
        }
    }

    // TODO: Better error code
    // Invalid cast, return error
    else {
        std::cerr << "Implicit cast\n";
    }
}

llvm::Value *CastTable::varCast(llvm::Type *type, llvm::Value *expr) {
    llvm::Value *exprLoad = ir->CreateLoad(expr);

    // GazpreaType of expr
    llvm::Type *exprType = exprLoad->getType();

    // Position in table
    int exprPos = getType(exprType);
    int typePos = getType(type);

    // Cast type
    std::string exprString = typeTable[exprPos][exprPos];
    std::string typeString = typeTable[typePos][typePos];

    // Casting expr to bool
    if(typeString == "bool"){
        if(exprString == "char") {

        }
        else if(exprString == "int") {

        }

    }

    // Casting expr to char
    else if(typeString == "char") {
        if(exprString == "bool") {

        }
        else if(exprString == "int") {

        }
    }

    // Casting expr to int
    else if(typeString == "int") {
        if(exprString == "bool") {

        }
        else if(exprString == "char") {

        }
        else if(exprString == "float") {
            ir->CreateFPToSI(exprLoad, type);
        }
    }

    // Casting expr to float
    else if(typeString == "float") {
        if(exprString == "bool") {

        }
        else if(exprString == "char") {

        }
        else if(exprString == "int") {

        }
    }

    // TODO: Better error code
    // Can't cast to type, return an error
    else {
        std::cerr << "Implicit cast\n";
    }
}
