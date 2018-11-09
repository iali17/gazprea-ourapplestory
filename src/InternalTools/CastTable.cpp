//
// Created by ali5 on 10/26/18.
//
#include <InternalTools/CastTable.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;

CastTable::CastTable(llvm::LLVMContext *globalctx, llvm::IRBuilder<> *ir, InternalTools *it, llvm::Module *mod, ErrorBuilder *eb) : globalCtx(globalctx), ir(ir), it(it), mod(mod), eb(eb) {
    // Do nothing
}

int CastTable::getType(llvm::Type *expr) {
    if(expr == i64Ty || expr == i32Ty || expr == intTy)
        return 2;
    else if(expr == realTy)
        return 3;
    else if(expr == boolTy)
        return 0;
    else if(expr == charTy || i8Ty)
        return 1;
    return -1;
}

// This function is to check cast promotion without the use of the keyword: as
InternalTools::pair CastTable::typePromotion(llvm::Value *lValueLoad, llvm::Value *rValueLoad, int line) {
    assert(!((lValueLoad == nullptr) && (rValueLoad == nullptr)));

    if(!lValueLoad)
        lValueLoad = it->getNull(rValueLoad->getType());
    else if(!rValueLoad)
        rValueLoad = it->getNull(lValueLoad->getType());

    if(lValueLoad->getName() == "IdnNode")
        lValueLoad = it->getIdn(rValueLoad->getType());
    else if(rValueLoad->getName() == "IdnNode")
        rValueLoad = it->getIdn(lValueLoad->getType());

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
        return it->makePair(lValueLoad, rValueLoad);
    }
    // Only viable cast for scalars: int -> real
    else if(castType == "real") {
        if(lTypeString == "int") {
            return it->makePair(ir->CreateSIToFP(lValueLoad, realTy, "upCastIntToReal"), rValueLoad);
        }
        else {
            return it->makePair(lValueLoad, ir->CreateSIToFP(rValueLoad, realTy, "upCastIntToreal"));
        }
    }
    else {
        auto *error = new ScalarNode(lTypeString, rTypeString, line);
        eb->printError(error);

        return it->makePair(lValueLoad, rValueLoad);
    }
}

// This function is for casting with the use of the keyword: as
llvm::Value *CastTable::varCast(llvm::Type *type, llvm::Value *exprLoad, int line) {
    auto trueValue = static_cast<uint64_t>(static_cast<int64_t>(0));
    llvm::Value *zero = llvm::ConstantInt::get(i32Ty, trueValue);

    // GazpreaType of expr
    llvm::Type *exprType = exprLoad->getType();

    // Position in table
    int exprPos = getType(exprType);
    int typePos = getType(type);

    // Option for explicit casting
    std::string exprString = typeTable[exprPos][exprPos];
    std::string typeString = typeTable[exprPos][typePos];
    std::string realString = typeTable[typePos][typePos];

    // Casting to same type
    if(typeString == exprString) {
        return exprLoad;
    }

    // Casting expr to bool
    else if(typeString == "bool"){
        if(exprString == "char") {
            llvm::Value *temp = ir->CreateZExt(exprLoad, intTy, "charToInt");
            return ir->CreateICmpNE(temp, zero, "charToBool");
        }
        else if(exprString == "int") {
            return ir->CreateICmpNE(exprLoad, zero, "intToBool");
        }
    }

    // Casting expr to char
    else if(typeString == "char") {
        if(exprString == "bool") {
            llvm::Value *temp = ir->CreateZExt(exprLoad, intTy, "boolToInt");
            return ir->CreateTrunc(temp, charTy, "intToChar");
        }
        else if(exprString == "int") {
            auto *cb = new CondBuilder(globalCtx, ir, mod);

            llvm::Value *exprStore = ir->CreateAlloca(intTy);
            ir->CreateStore(exprLoad, exprStore);

            // If integer is negative, perform subtraction
            // Otherwise, skip
            llvm::Value *cond = ir->CreateICmpSLT(exprLoad, zero);
            cb->beginIf(cond, "intToChar");;

            llvm::Value *temp = ir->CreateSub(zero, exprLoad);
            ir->CreateStore(temp, exprStore);
            cb->endIf();
            cb->finalize("intToChar");

            exprLoad = ir->CreateLoad(exprStore);
            return ir->CreateTrunc(exprLoad, charTy, "intToChar");
        }
    }

    // Casting expr to int
    else if(typeString == "int") {
        if(exprString == "bool") {
            return ir->CreateZExt(exprLoad, intTy, "boolToInt");
        }
        else if(exprString == "char") {
            return ir->CreateSExt(exprLoad, intTy, "charToInt");
        }
        else if(exprString == "real") {
            return ir->CreateFPToSI(exprLoad, intTy, "realToInt");
        }
    }

    // Casting expr to real
    else if(typeString == "real") {
        if(exprString == "bool") {
            return ir->CreateUIToFP(exprLoad, realTy, "boolToReal");
        }
        else if(exprString == "char") {
            return ir->CreateUIToFP(exprLoad, realTy, "charToReal");
        }
        else if(exprString == "int") {
            return ir->CreateSIToFP(exprLoad, realTy, "intToReal");
        }
    }

    else {
        auto *error = new ScalarNode(realString, exprString, line);
        eb->printError(error);
    }
    return nullptr;
}

llvm::Value *CastTable::typeAssCast(llvm::Type *type, llvm::Value *expr, int line) {
    if(expr->getName() == "IdnNode")
        expr = it->getIdn(type);

    llvm::Type *rTypeP = expr->getType();

    // Position in typeTable
    int lType = getType(type);
    int rType = getType(rTypeP);

    // Cast type
    std::string castType = typeAssTable[lType][rType];
    std::string lTypeString = typeAssTable[lType][lType];
    std::string rTypeString = typeAssTable[rType][rType];

    if(castType == lTypeString && castType == rTypeString) {
        return expr;
    }
    // Only viable cast for scalars: int -> real
    else if(castType == "real") {
        return ir->CreateSIToFP(expr, realTy, "upCastIntToReal");
    }
    else {
        auto *error = new ScalarNode(lTypeString, rTypeString, line);
        eb->printError(error);
    }
    return nullptr;
}
