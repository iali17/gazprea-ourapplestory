//
// Created by kyle on 20/10/18.
//

#include <CodeGenerator/CodeGenerator.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;

llvm::Value *CodeGenerator::visit(FileNode *node) {
    // register external functions
    et->registerPrintf();
    et->registerFree();
    et->registerCalloc();
    et->registerScanf();

    symbolTable->pushNewScope("_globalScope_");

    //add base types
    symbolTable->addBaseType("boolean"  , boolTy);
    symbolTable->addBaseType("character", charTy);
    symbolTable->addBaseType("integer"  , intTy);
    symbolTable->addBaseType("real"     , realTy);

    unsigned long i = 0;
    for(i = 0; i < node->nodes->size(); i++){
        ASTBaseVisitor::visit(node->nodes->at(i));
    }

    symbolTable->popScope();
    return nullptr;
}

llvm::Value *CodeGenerator::visit(BasicBlockNode *node) {
    symbolTable->pushNewScope();
    unsigned long i = 0;
    for(i = 0; i < node->nodes->size(); i++){
        ASTBaseVisitor::visit(node->nodes->at(i));
    }
    symbolTable->popScope();
    return nullptr;
}

llvm::Value *CodeGenerator::visit(ASTNode *node) {
    return ASTBaseVisitor::visit(node);
}

CodeGenerator::CodeGenerator(char *outFile) : outFile(outFile) {
}

llvm::Value *CodeGenerator::visit(ProcedureNode *node) {
    //TODO - BUILD FUNCTION TYPE PROPERLY
    llvm::FunctionType *funcTy = llvm::TypeBuilder<int(), false>::get(*globalCtx);
    //TODO - BEFORE PUSHING SCOPE ADD FUNCTION DECL
    symbolTable->pushNewScope();
    //TODO - REGISTER THOSE VARIABLES
    //llvm::ArrayRef<llvm::GazpreaType *> *params =  new llvm::ArrayRef<llvm::GazpreaType *>;
    //llvm::FunctionType::get(intTy, params);

    auto *func = llvm::cast<llvm::Function>(mod->getOrInsertFunction(node->getProcedureName(), funcTy));
    // Create an entry block and set the inserter.
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(*globalCtx, "entry", func);
    ir->SetInsertPoint(entry);

    visit(node->getBlock());

    symbolTable->popScope();
    return nullptr;
}

llvm::Value *CodeGenerator::visit(ParamNode *node) {
    return ASTBaseVisitor::visit(node);
}

llvm::Value *CodeGenerator::visit(ReturnNode *node) {
    if (node->getExpr() == nullptr){
        ir->CreateRetVoid();
    }
    else {
        llvm::Value *ret = visit(node->getExpr());
        ir->CreateRet(ret);
    }
    return nullptr;
}

llvm::Value *CodeGenerator::visit(INTNode *node) {
    return it->getConsi32(node->value);
}

llvm::Value *CodeGenerator::visit(RealNode *node) {
    double val = node->getVal();
    return it->getReal(val);
}

llvm::Value *CodeGenerator::visit(CharNode *node) {
    char val = node->getVal();
    return it->geti8(val);
}

llvm::Value *CodeGenerator::visit(BoolNode *node) {
    bool val = node->getVal();
    return it->geti1(val);
}

llvm::Value *CodeGenerator::visit(CondNode *node) {
    CondBuilder *condBuilder = new CondBuilder(globalCtx, ir, mod);

    unsigned long i = 0;
    for(i = 0; i < node->getConds()->size(); i++){
        if(i){
            condBuilder->createElseIf(visit(node->getConds()->at(i)));
        }
        else {
            condBuilder->createIf(visit(node->getConds()->at(i)));
        }
        visit(node->getBlocks()->at(i));
    }

    if(node->isHasElse()){
        condBuilder->createElse();
        visit(node->getBlocks()->at(i));
    }
    condBuilder->finalize();
    return nullptr;
}

llvm::Value *CodeGenerator::visit(LoopNode *node) {
    WhileBuilder *whileBuilder = new WhileBuilder(globalCtx, ir, mod);
    whileBuilder->beginWhile();
    if(node->getControl())
        whileBuilder->insertControl(visit(node->getControl()));
    else
        whileBuilder->insertControl(it->geti1(1));
    visit(node->getBlock());
    whileBuilder->endWhile();
    return nullptr;
}

llvm::Value *CodeGenerator::visit(DoLoopNode *node) {
    WhileBuilder *whileBuilder = new WhileBuilder(globalCtx, ir, mod);
    whileBuilder->beginWhile();
    visit(node->getBlock());
    whileBuilder->insertControl(visit(node->getControl()));
    whileBuilder->endWhile();
    return nullptr;
}

llvm::Value *CodeGenerator::visit(InLoopNode *node) {
    //todo - hopefully we can handle the iterator (in) in it's own node
    //                THEN WE CAN DELETE THIS CLASS
    return nullptr;
}

llvm::Value *CodeGenerator::visit(DeclNode *node) {
    //TODO - account for null
    llvm::Value *val = visit(node->getExpr());

    if      (node->getTypeIds()->size() == 0){
        llvm::Value* ptr = ir->CreateAlloca(val->getType());
        ir->CreateStore(val, ptr);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    }
    else if (node->getTypeIds()->size() == 1){
        llvm::Value* ptr = ir->CreateAlloca(symbolTable->resolveType(node->getTypeIds()->at(0))->getTypeDef());
        ir->CreateStore(val, ptr);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    }
    //TODO - ALL OTHER CASES
    return nullptr;
}

llvm::Value *CodeGenerator::visit(AssignNode *node) {
    return nullptr;
}
