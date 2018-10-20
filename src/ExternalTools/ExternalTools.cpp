//
// Created by kyle on 20/10/18.
//

#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/IRBuilder.h>
#include <ExternalTools/ExternalTools.h>

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
    auto *freeFunc        = llvm::cast<llvm::Function>(mod->getOrInsertFunction("free", fTy));
}

void ExternalTools::aliFree(llvm::Value *p) {
    llvm::Value *charP = ir->CreatePointerCast(p, charTy->getPointerTo());
    llvm::Function *myFree = mod->getFunction("free");
    llvm::Value *x = ir->CreateCall(myFree, {charP});
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
    llvm::Constant *intFormatStr  = llvm::ConstantDataArray::getString(*globalCtx, "%d");
    llvm::Constant *charFormatStr = llvm::ConstantDataArray::getString(*globalCtx, "%c");
    llvm::Constant *spaceStr      = llvm::ConstantDataArray::getString(*globalCtx, " ");
    llvm::Constant *eolnStr       = llvm::ConstantDataArray::getString(*globalCtx, "\n");
    llvm::Constant *openSqrBStr   = llvm::ConstantDataArray::getString(*globalCtx, "[");
    llvm::Constant *closeSqrBStr  = llvm::ConstantDataArray::getString(*globalCtx, "]");

    // Create the global space we will use. The string "intFormatStr" is the name you will need to
    // to use to ask for this value later to get it from the module.
    auto *intFormatStrLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(INTFORMAT_STR, intFormatStr->getType())
            );

    auto *charFormatStrLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(CHARFORMAT_STR, charFormatStr->getType())
            );

    // Set the location to be initialised by the constant.
    intFormatStrLoc->setInitializer(intFormatStr);

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
                    mod->getOrInsertGlobal(OPENSQRB_STR, openSqrBStr->getType())
            );

    // Set the location to be initialised by the constant.
    openSqrBLoc->setInitializer(openSqrBStr);

    auto *closeSqrBLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(CLOSESQRB_STR, closeSqrBStr->getType())
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
    auto *formatStrGlobal = llvm::cast<llvm::Value>(mod->getGlobalVariable(INTFORMAT_STR));

    // The type of your string will be [n x i8], it needs to be i8*, so we cast here. We
    // explicitly use the type of printf's first arg to guarantee we are always right.
    llvm::Value *formatStr =
            ir->CreatePointerCast(formatStrGlobal, printfFunc->arg_begin()->getType(), "formatStr");

    // Call printf
    ir->CreateCall(printfFunc, {formatStr, val});
}

/**
 * This function prints a string.
 * @param str - pointer to character vector
 */
void ExternalTools::printStr(llvm::Value *str) {
    llvm::Function *printfFunc = mod->getFunction("printf");

    // Get your string to print.
    auto *formatStrGlobal = llvm::cast<llvm::Value>(mod->getGlobalVariable(CHARFORMAT_STR));

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
    auto *formatStrGlobal = llvm::cast<llvm::Value>(mod->getGlobalVariable(CHARFORMAT_STR));

    // Call printf. Printing multiple values is easy: just add to the {}.
    llvm::Value *formatStr =
            ir->CreatePointerCast(formatStrGlobal, printfFunc->arg_begin()->getType());

    ir->CreateCall(printfFunc, {formatStr, ch});
}