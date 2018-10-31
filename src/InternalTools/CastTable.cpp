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

llvm::Value *CastTable::typePromotion(llvm::Value *lValueLoad, llvm::Value *rValueLoad) {
    //llvm::Value *lValueLoad = ir->CreateLoad(leftExpr);
    //llvm::Value *rValueLoad = ir->CreateLoad(rightExpr);

    // Gazprea type of left and right expr
    llvm::Type *lTypeP = lValueLoad->getType();
    llvm::Type *rTypeP = rValueLoad->getType();

    // Position in typeTable
    int lType = getType(lTypeP);
    int rType = getType(rTypeP);

    // Cast type
    std::string castType = typePTable[lType][rType];
    std::string lTypeString = typePTable[lType][lType];
    std::string rTypeString = typePTable[rType][rType];

    if(castType == lTypeString && castType == rTypeString) {
        std::cout << "No conversion necessary\n";
    }
    // Only viable cast for scalars: int -> real
    else if(castType == "float") {
        if(lTypeString == "int") {
            ir->CreateSIToFP(lValueLoad, realTy, "upCastIntToReal");
        }
        else {
            ir->CreateSIToFP(rValueLoad, realTy, "upCastIntToreal");
        }
    }
    else {
        std::cerr << "No implicit conversion possible\n";
    }


    // MAYBE DELETE? since the only type promotion is int->float
    // If kyle is doing it in the ast is there a need for this function.
    /*
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
        else {
            std::cout << "Cast unnecessary\n";
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
        else {
            std::cout << "Cast unnecessary\n";
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
        else {
            std::cout << "Cast unnecessary\n";
        }
    }

    // TODO: Better error code
    // Invalid cast, return error
    else {
        std::cerr << "Implicit cast\n";
    }

     */
}

llvm::Value *CastTable::varCast(llvm::Type *type, llvm::Value *exprLoad) {
    uint64_t trueValue = static_cast<uint64_t>(static_cast<int64_t>(0));
    llvm::Value *zero = llvm::ConstantInt::get(i32Ty, trueValue);

    // TODO: Gotta fix casting, giving a segfault

    // Value of expr
    //llvm::Value *exprLoad = ir->CreateLoad(expr);

    // GazpreaType of expr
    llvm::Type *exprType = exprLoad->getType();

    // Position in table
    int exprPos = getType(exprType);
    int typePos = getType(type);

    // Cast type
    std::string exprString = typeTable[exprPos][exprPos];
    std::string typeString = typeTable[exprPos][typePos];

    // Casting expr to bool
    if(typeString == "bool"){
        if(exprString == "char") {
            ir->CreateSExt(exprLoad, intTy);
            ir->CreateICmpNE(exprLoad, zero, "charToBool");
        }
        else if(exprString == "int") {
            ir->CreateICmpNE(exprLoad, zero, "intToBool");
        }
    }

    // Casting expr to char
    else if(typeString == "char") {
        if(exprString == "bool") {
            ir->CreateSExt(exprLoad, intTy, "boolToInt");
            ir->CreateTrunc(exprLoad, charTy, "intToChar");
        }
        else if(exprString == "int") {
            ir->CreateTrunc(exprLoad, charTy, "intToChar");
        }
    }

    // Casting expr to int
    else if(typeString == "int") {
        if(exprString == "bool") {
            ir->CreateSExt(exprLoad, intTy, "boolToInt");
        }
        else if(exprString == "char") {
            ir->CreateSExt(exprLoad, intTy, "charToInt");
        }
        else if(exprString == "float") {
            ir->CreateFPToSI(exprLoad, intTy, "floatToInt");
        }
    }

    // Casting expr to float
    else if(typeString == "float") {
        if(exprString == "bool") {
            ir->CreateFPToSI(exprLoad, intTy);
            ir->CreateICmpNE(exprLoad, zero, "floatToBool");
        }
        else if(exprString == "char") {
            ir->CreateSExt(exprLoad, intTy, "charToInt");
            ir->CreateSIToFP(exprLoad, realTy, "intToFloat");
        }
        else if(exprString == "int") {
            //need to return the casted value, other wise the casted value is never used
            return ir->CreateSIToFP(exprLoad, realTy, "intToFloat");
        }
    }

    // TODO: Better error code
    // Can't cast to type, return an error
    else {
        std::cerr << "Implicit cast\n";
    }
}
