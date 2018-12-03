//
// Created by kyle on 20/10/18.
//
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <ExternalTools/ExternalTools.h>
#include <InternalTools/CondBuilder.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *strTy;
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
extern llvm::Type *streamStateTy;

ExternalTools::ExternalTools(llvm::LLVMContext *globalCtx, llvm::IRBuilder<> *ir, llvm::Module *mod) : globalCtx(
        globalCtx), ir(ir), mod(mod) {
}

void ExternalTools::registerCalloc() {
    // Declare calloc. Returns char *, takes array size, element size.
    llvm::FunctionType *fTy = llvm::TypeBuilder<char *(size_t, size_t), false>::get(*globalCtx);
    auto *callocFunc        = llvm::cast<llvm::Function>(mod->getOrInsertFunction("calloc", fTy));

    // Add the suggested function attributes.
    callocFunc->addFnAttr(llvm::Attribute::NoUnwind);
    callocFunc->addAttribute(0, llvm::Attribute::NoAlias);
}

/**
 * call calloc. returns a pointer to integer array
 *
 * @param arrSize
 * @param elementSize
 * @param castToIntP
 * @return i32* or i8*
 */
llvm::Value *ExternalTools::aliCalloc(llvm::Value *arrSize, int elementSize, bool castToIntP) {
    auto trueValue       = static_cast<uint64_t>(static_cast<int64_t>(elementSize));
    llvm::Value *elmSize = llvm::ConstantInt::get(i64Ty, trueValue, true);
    llvm::Value *length  = ir->CreateIntCast(arrSize, i64Ty, true);

    // get the function and create call
    llvm::Function *myCalloc = mod->getFunction("calloc");
    llvm::Value *x = ir->CreateCall(myCalloc, {length, elmSize});

    if (castToIntP)
        return ir->CreatePointerCast(x, intTy->getPointerTo());
    return x;
}

void ExternalTools::registerFree() {
    // Declare calloc. Returns char *, takes array size, element size.
    llvm::FunctionType *fTy = llvm::TypeBuilder<void (void *), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("free", fTy));
}

void ExternalTools::aliFree(llvm::Value *p) {
    llvm::Value *charP = ir->CreatePointerCast(p, charTy->getPointerTo());
    llvm::Function *myFree = mod->getFunction("free");
    ir->CreateCall(myFree, {charP});
}

void ExternalTools::registerPow() {
    llvm::FunctionType *fTy = llvm::TypeBuilder<double (double, double), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("pow", fTy));
}

llvm::Value *ExternalTools::aliPow(llvm::Value *left, llvm::Value *right) {
    llvm::Value *dblLeft;
    llvm::Value *dblRight;
    llvm::Value *ret;

    // get the function and create call
    llvm::Function *myPow = mod->getFunction("pow");

    if(left->getType() == intTy){
        dblLeft  = ir->CreateSIToFP(left,  llvm::Type::getDoubleTy(*globalCtx));
        dblRight = ir->CreateSIToFP(right, llvm::Type::getDoubleTy(*globalCtx));
        ret = ir->CreateCall(myPow, {dblLeft, dblRight});
        ret = ir->CreateFPToSI(ret, intTy);

    } else {
        dblLeft  = ir->CreateFPExt(left,  llvm::Type::getDoubleTy(*globalCtx));
        dblRight = ir->CreateFPExt(right, llvm::Type::getDoubleTy(*globalCtx));
        ret = ir->CreateCall(myPow, {dblLeft, dblRight});
        ret = ir->CreateFPCast(ret, realTy);
    }
    return ret;
}

/**
 * register printf and create global string constants
 */
void ExternalTools::registerPrintf() {
    llvm::FunctionType *printfTy = llvm::TypeBuilder<int(char *, ...), false>::get(*globalCtx);
    auto *printfFunc             = llvm::cast<llvm::Function>(mod->getOrInsertFunction("printf", printfTy));

    printfFunc->addAttribute(1u, llvm::Attribute::NoAlias);
    printfFunc->addAttribute(1u, llvm::Attribute::NoCapture);

    // Create the constant data array of characters.
    llvm::Constant *intFormatStr   = llvm::ConstantDataArray::getString(*globalCtx, "%d");
    llvm::Constant *charFormatStr  = llvm::ConstantDataArray::getString(*globalCtx, "%c");
    llvm::Constant *floatFormatStr = llvm::ConstantDataArray::getString(*globalCtx, "%g");
    llvm::Constant *spaceStr       = llvm::ConstantDataArray::getString(*globalCtx, " ");
    llvm::Constant *eolnStr        = llvm::ConstantDataArray::getString(*globalCtx, "\n");
    llvm::Constant *openSqrBStr    = llvm::ConstantDataArray::getString(*globalCtx, "[");
    llvm::Constant *closeSqrBStr   = llvm::ConstantDataArray::getString(*globalCtx, "]");
    llvm::Constant *trueStr        = llvm::ConstantDataArray::getString(*globalCtx, "T");
    llvm::Constant *falseStr       = llvm::ConstantDataArray::getString(*globalCtx, "F");

    // Create the global space we will use. The string "intFormatStr" is the name you will need to
    // to use to ask for this value later to get it from the module.
    auto *intFormatStrLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(INT_FORMAT_STR, intFormatStr->getType())
            );

    intFormatStrLoc->setInitializer(intFormatStr);

    auto *charFormatStrLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(CHAR_FORMAT_STR, charFormatStr->getType())
            );

    // Set the location to be initialised by the constant.
    charFormatStrLoc->setInitializer(charFormatStr);

    auto *trueFormatStrLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(TRUE_STR, trueStr->getType())
            );

    trueFormatStrLoc->setInitializer(trueStr);

    auto *falseFormatStrLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(FALSE_STR, falseStr->getType())
            );

    // Set the location to be initialised by the constant.
    falseFormatStrLoc->setInitializer(falseStr);

    auto *floatFormatStrLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(FLOAT_FORMAT_STR, floatFormatStr->getType())
            );

    // Set the location to be initialised by the constant.
    floatFormatStrLoc->setInitializer(floatFormatStr);

    auto *spaceStrLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(SPACE_STR, spaceStr->getType())
            );

    // Set the location to be initialised by the constant.
    spaceStrLoc->setInitializer(spaceStr);

    auto *eolnStrLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(EOLN_STR, eolnStr->getType())
            );

    // Set the location to be initialised by the constant.
    eolnStrLoc->setInitializer(eolnStr);

    auto *openSqrBLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(OPEN_SQRB_STR, openSqrBStr->getType())
            );

    // Set the location to be initialised by the constant.
    openSqrBLoc->setInitializer(openSqrBStr);

    auto *closeSqrBLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(CLOSE_SQRB_STR, closeSqrBStr->getType())
            );

    // Set the location to be initialised by the constant.
    closeSqrBLoc->setInitializer(closeSqrBStr);
}

/**
 * print a pre-defined string. see string constants
 * @param constStr
 */
void ExternalTools::printStaticStr(std::string constStr) {
    llvm::Function *printfFunc = mod->getFunction("printf");

    // Get your string to print.
    auto *formatStrGlobal = llvm::cast<llvm::Value>(mod->getGlobalVariable(constStr));

    // Call printf. Printing multiple values is easy: just add to the {}.
    llvm::Value *formatStr =
            ir->CreatePointerCast(formatStrGlobal, printfFunc->arg_begin()->getType());

    ir->CreateCall(printfFunc, {formatStr});
}

/**
 * print a single int
 * @param val - i32
 */
void ExternalTools::printInt(llvm::Value *val) {
    // get the function
    llvm::Function *printfFunc = mod->getFunction("printf");

    // Get your string to print.
    auto *formatStrGlobal = llvm::cast<llvm::Value>(mod->getGlobalVariable(INT_FORMAT_STR));

    // The type of your string will be [n x i8], it needs to be i8*, so we cast here. We
    // explicitly use the type of printf's first arg to guarantee we are always right.
    llvm::Value *formatStr =
            ir->CreatePointerCast(formatStrGlobal, printfFunc->arg_begin()->getType(), "formatStr");

    // Call printf
    ir->CreateCall(printfFunc, {formatStr, val});
}

/**
 * This function prints a string. This function may not get used since we may want to call print char
 * while generating the code
 * @param str - pointer to character vector
 */
void ExternalTools::printStr(llvm::Value *str) {
    llvm::Function *printfFunc = mod->getFunction("printf");

    // Get your string to print.
    auto *formatStrGlobal = llvm::cast<llvm::Value>(mod->getGlobalVariable(CHAR_FORMAT_STR));

    // Call printf. Printing multiple values is easy: just add to the {}.
    llvm::Value *formatStr =
            ir->CreatePointerCast(formatStrGlobal, printfFunc->arg_begin()->getType());

    //TODO - loop and call print for each character until NULL is reached

    ir->CreateCall(printfFunc, {formatStr});
}

/**
 * prints a char
 * @param ch - i8
 */
void ExternalTools::printChar(llvm::Value *ch) {
    llvm::Function *printfFunc = mod->getFunction("printf");

    // Get your string to print.
    auto *formatStrGlobal = llvm::cast<llvm::Value>(mod->getGlobalVariable(CHAR_FORMAT_STR));

    // Call printf. Printing multiple values is easy: just add to the {}.
    llvm::Value *formatStr =
            ir->CreatePointerCast(formatStrGlobal, printfFunc->arg_begin()->getType());

    ir->CreateCall(printfFunc, {formatStr, ch});
}

void ExternalTools::printBoolean(llvm::Value *val) {
    //bless the condbuilder
    CondBuilder *condBuilder = new CondBuilder(globalCtx, ir, mod);

    condBuilder->beginIf(val);
        printStaticStr(TRUE_STR);
    condBuilder->endIf();
    condBuilder->beginElse();
        printStaticStr(FALSE_STR);
    condBuilder->finalize();
}

/**
 * printf a float
 * @param val - llvm float type
 */
void ExternalTools::printReal(llvm::Value *val) {
    llvm::Function *printfFunc = mod->getFunction("printf");

    // Get your string to print.
    auto *formatStrGlobal = llvm::cast<llvm::Value>(mod->getGlobalVariable(FLOAT_FORMAT_STR));

    // Call printf. Printing multiple values is easy: just add to the {}.
    llvm::Value *formatStr =
            ir->CreatePointerCast(formatStrGlobal, printfFunc->arg_begin()->getType());

    ir->CreateCall(printfFunc, {formatStr, ir->CreateFPExt(val, llvm::Type::getDoubleTy(*globalCtx))});
}

void ExternalTools::registerScanf() {
    llvm::FunctionType *fTy = llvm::TypeBuilder<void (void *, void*, int), false>::get(*globalCtx);
    llvm::cast<llvm::Function>(mod->getOrInsertFunction("read_in", fTy));
}

/**
 * @param scanString - the string used for reading, USE THE PREDEFINED CONSTANT STRINGS
 * @return pointer to the read value or null otherwise
 */
llvm::Value *ExternalTools::aliScanf(llvm::Value *stream, llvm::Value *scanTo, llvm::Value *type) {
    llvm::Function *scanfFunc = mod->getFunction("read_in");

    // Get your string to print.
    //auto *formatStrGlobal = llvm::cast<llvm::Value>(mod->getGlobalVariable(constScanString));

    // Call printf. Printing multiple values is easy: just add to the {}.
    //llvm::Value *formatStr =
     //       ir->CreatePointerCast(formatStrGlobal, scanfFunc->arg_begin()->getType());

    //TODO - check stream state

    llvm::Value *vStream = ir->CreatePointerCast(stream, charTy->getPointerTo());
    llvm::Value *vDest = ir->CreatePointerCast(scanTo, charTy->getPointerTo());

    ir->CreateCall(scanfFunc, {vStream, vDest, type});

    return nullptr;
}

void ExternalTools::print(llvm::Value *val) {
    llvm::Type * llvmType = val->getType();
    if(llvmType->isIntegerTy(32)){
        printInt(val);
    }
    else if(llvmType->isIntegerTy(8)){
        printChar(val);
    }
    else if(llvmType->isIntegerTy(1)){
        printBoolean(val);
    }
    else if(llvmType->isFloatTy()){
        printReal(val);
    }
    else if(llvmType == strTy->getPointerTo()) {
       printVectorAsString(val);
    }
    else if(llvmType == boolVecTy->getPointerTo() ||
            llvmType == charVecTy->getPointerTo() ||
            llvmType == intVecTy->getPointerTo()  ||
            llvmType == realVecTy->getPointerTo() ){
        printVector(val);
    }
    else if(llvmType == boolMatrixTy->getPointerTo() ||
            llvmType == charMatrixTy->getPointerTo() ||
            llvmType == intMatrixTy->getPointerTo()  ||
            llvmType == realMatrixTy->getPointerTo() ){
        printMatrix(val);
    }
}
