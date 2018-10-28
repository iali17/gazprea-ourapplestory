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
    symbolTable->addBaseType("void"     , llvm::Type::getVoidTy(*globalCtx));

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
    // http://releases.llvm.org/3.5.2/docs/tutorial/LangImpl3.html#function-code-generation

    //TODO - BUILD FUNCTION TYPE PROPERLY
    std::vector<ASTNode *>  paramsList = *node->getParamNodes();
    std::vector<llvm::Type *> params;
    std::cout << "what in the fuck " << node->getRetType() << "\n";

    llvm::Type *retType = symbolTable->resolveType(node->getRetType())->getTypeDef();

    for (auto it = paramsList.begin(); it!= paramsList.end(); ++it) {
        params.push_back(it.operator*()->getLlvmType());
        std::cout << ((ParamNode *) it.operator*())->getDeclaredType()  << "\n";
    }

    llvm::FunctionType *funcTy = llvm::FunctionType::get(retType, params, false);

    std::cout <<"this is it " << funcTy->getFunctionNumParams() << std::endl;

    llvm::Function *F = llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage, node->getProcedureName(), mod);

    if (F->getName() != node->getProcedureName()) {
        printf("yes?\n");
        F->eraseFromParent();
        F = mod->getFunction(node->getProcedureName());

        if (!F->empty()) {
            //TODO: error message
            std::cout << "redefinition of function" << "\n";
            return nullptr;
        }

        if(F->arg_size() != paramsList.size()) {
            //TODO: error message
            std::cout << "redefinition of function with different # args" << "\n";
            return nullptr;
        }
    }


    //TODO - BEFORE PUSHING SCOPE ADD FUNCTION
    symbolTable->pushNewScope();
    printf("fuck\n");
    //TODO - REGISTER THOSE VARIABLES
    llvm::Value* ptr = nullptr;
    llvm::Value* val = nullptr;
    unsigned Idx = 0;
    for (llvm::Function::arg_iterator AI = F->arg_begin(); Idx != paramsList.size(); ++AI, ++Idx){
        
    }

    //llvm::ArrayRef<llvm::GazpreaType *> *params =  new llvm::ArrayRef<llvm::GazpreaType *>;
    //llvm::FunctionType::get(intTy, params);

    //auto *func = llvm::cast<llvm::Function>(mod->getOrInsertFunction(node->getProcedureName(), funcTy));
    // Create an entry block and set the inserter.

    llvm::BasicBlock *entry = llvm::BasicBlock::Create(*globalCtx, "entry", F);
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
    llvm::Value* ptr = nullptr;

    if      (node->getTypeIds()->size() == 0){
        ptr = ir->CreateAlloca(val->getType());
        ir->CreateStore(val, ptr);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    }
    else if (node->getTypeIds()->size() == 1){
        ptr = ir->CreateAlloca(symbolTable->resolveType(node->getTypeIds()->at(0))->getTypeDef());
        ir->CreateStore(val, ptr);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    }

    symbolTable->resolveSymbol(node->getID())->setPtr(ptr);
    //TODO - ALL OTHER CASES
    return nullptr;
}

llvm::Value *CodeGenerator::visit(AssignNode *node) {
    llvm::Value *val = visit(node->getExpr());
    //TODO - IMPLICIT UPCAST WHEN NEEDED
    llvm::Value *ptr = symbolTable->resolveSymbol(node->getID())->getPtr();
    ir->CreateStore(val, ptr);
    return nullptr;
}

llvm::Value *CodeGenerator::visit(IDNode *node) {
    llvm::Value *ptr = symbolTable->resolveSymbol(node->getID())->getPtr();
    return ir->CreateLoad(ptr);
}

llvm::Value *CodeGenerator::visit(InputNode *node) {
    if(symbolTable->resolveSymbol(node->getStreamName())->getType() != INSTREAM){
        std::cerr << "Not proper stream\nAborting...\n";
        exit(1);
    }
    llvm::Value *ptr = symbolTable->resolveSymbol(node->getStoreID())->getPtr();
    et->aliScanf(ptr);
    return nullptr;
}

llvm::Value *CodeGenerator::visit(OutputNode *node) {
    if(symbolTable->resolveSymbol(node->getStreamName())->getType() != OUTSTREAM){
        std::cerr << "Not proper stream\nAborting...\n";
        exit(1);
    }
    llvm::Value *expr = visit(node->getExpr());
    et->print(expr);
    return nullptr;
}

llvm::Value *CodeGenerator::visit(StreamDeclNode *node) {
    symbolTable->addSymbol(node->getId(), node->getType(), false);
    return nullptr;
}
