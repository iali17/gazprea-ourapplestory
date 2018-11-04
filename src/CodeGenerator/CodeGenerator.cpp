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

    std::vector<ASTNode *>  paramsList = *node->getParamNodes();
    std::vector<llvm::Type *> params;

    llvm::Type *retType = symbolTable->resolveType(node->getRetType())->getTypeDef();

    for (auto it = paramsList.begin(); it!= paramsList.end(); ++it) {
        std::string typeName =  ((ParamNode *) it.operator*())->getDeclaredType();

        params.push_back(symbolTable->resolveType(typeName)->getTypeDef()->getPointerTo());
    }

    llvm::FunctionType *funcTy = llvm::FunctionType::get(retType, params, false);

    llvm::Function *F = llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage, node->getProcedureName(), mod);

    if (F->getName() != node->getProcedureName()) {
        F->eraseFromParent();
        F = mod->getFunction(node->getProcedureName());

        if (!F->empty()) {
            //TODO: error message
            std::cerr << "redefinition of function" << "\n";
            exit(1);
        }

        if(F->arg_size() != paramsList.size()) {
            //TODO: error message
            std::cerr << "redefinition of function with different # args" << "\n";
            exit(1);
        }
    }

    //new scope
    symbolTable->pushNewScope();

    //preserve old while stack
    auto *oldWhileStack = whileStack;
    whileStack = new std::stack<WhileBuilder *>;

    size_t idx = 0;
    for (auto AI = F->arg_begin(); idx != F->arg_size(); ++AI, ++idx){
        auto *p = (ParamNode *) paramsList.at(idx);
        symbolTable->addSymbol(p->getVarName(), p->getType(), false, AI);
        AI->setName(p->getVarName());
    }


    // Create an entry block and set the inserter.

    llvm::BasicBlock *entry = llvm::BasicBlock::Create(*globalCtx, "entry", F);
    ir->SetInsertPoint(entry);

    visit(node->getBlock());

    symbolTable->popScope();
    whileStack = oldWhileStack;
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
    float val = node->getVal();
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
            llvm::Value *cond = visit(node->getConds()->at(i));
            condBuilder->beginElseIf(ct->varCast(boolTy, cond));
        }
        else {
            llvm::Value *cond = visit(node->getConds()->at(i));
            condBuilder->beginIf(ct->varCast(boolTy, cond));
        }
        visit(node->getBlocks()->at(i));
        condBuilder->endIf();
    }

    if(node->isHasElse()){
        condBuilder->beginElse();
        visit(node->getBlocks()->at(i));
    }
    condBuilder->finalize();
    return nullptr;
}

llvm::Value *CodeGenerator::visit(LoopNode *node) {
    WhileBuilder *whileBuilder = new WhileBuilder(globalCtx, ir, mod);
    whileStack->push(whileBuilder);
    whileBuilder->beginWhile();
    if(node->getControl()) {
        llvm::Value *cond = visit(node->getControl());
        whileBuilder->insertControl(ct->varCast(boolTy, cond));
    }
    else
        whileBuilder->insertControl(it->geti1(1));

    visit(node->getBlock());
    whileBuilder->endWhile();
    whileStack->pop();
    return nullptr;
}

llvm::Value *CodeGenerator::visit(DoLoopNode *node) {
    WhileBuilder *whileBuilder = new WhileBuilder(globalCtx, ir, mod);
    whileStack->push(whileBuilder);
    whileBuilder->beginWhile();
    visit(node->getBlock());
    llvm::Value *cond = visit(node->getControl());
    whileBuilder->insertControl(ct->varCast(boolTy, cond));
    whileBuilder->endWhile();
    whileStack->pop();
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
        llvm::Type *type = symbolTable->resolveType(node->getTypeIds()->at(0))->getTypeDef();
        ptr = ir->CreateAlloca(type);
        if(val == nullptr) {
            if (!(it->setNull(type, ptr))){
                std::cerr << "Unable to initialize to null\n";
            }
            symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), ptr);
            return nullptr;
        }
        ir->CreateStore(val, ptr);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    }

    symbolTable->resolveSymbol(node->getID())->setPtr(ptr);
    //TODO - ALL OTHER CASES
    return nullptr;
}

llvm::Value *CodeGenerator::visit(AssignNode *node) {
    llvm::Value *ptr = symbolTable->resolveSymbol(node->getID())->getPtr();
    llvm::Value *val = visit(node->getExpr());

    if(val) {
        if(ptr->getType()->getPointerElementType() == realTy)
            val = ct->varCast(realTy, val);

        ir->CreateStore(val, ptr);
    }
    else if (!(it->setNull(ptr->getType()->getPointerElementType(), ptr))){
            std::cerr << "Unable to initialize to null\n";
    }
    return nullptr;
}

llvm::Value *CodeGenerator::visit(IDNode *node) {
    if(node->getID() == "null"){
        return nullptr;
    }

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
    et->printStaticStr(EOLN_STR);
    return nullptr;
}

llvm::Value *CodeGenerator::visit(StreamDeclNode *node) {
    symbolTable->addSymbol(node->getId(), node->getType(), false);
    return nullptr;
}

llvm::Value *CodeGenerator::visit(CallNode *node) {
    llvm::Function *func = mod->getFunction(node->getProcedureName());
    std::vector<llvm::Value *> dumb;

    // TODO: All the other types :(
    for (unsigned int i = 0; i < node->getExprNodes()->size(); ++i) {
        if ((node->getExprNodes()->at(i)->getType()) == CHAR) {
            llvm::Value* ptr = ir->CreateAlloca(charTy);
            llvm::Value* val = visit(node->getExprNodes()->at(i));
            ir->CreateStore(val, ptr);
            dumb.push_back(ptr);
        } else if ((node->getExprNodes()->at(i)->getType()) == INTEGER) {
            llvm::Value* ptr = ir->CreateAlloca(intTy);
            llvm::Value* val = visit(node->getExprNodes()->at(i));
            ir->CreateStore(val, ptr);
            dumb.push_back(ptr);
        }  else if ((node->getExprNodes()->at(i)->getType()) == REAL) {
            llvm::Value *ptr = ir->CreateAlloca(realTy);
            llvm::Value *val = visit(node->getExprNodes()->at(i));
            ir->CreateStore(val, ptr);
            dumb.push_back(ptr);
        } else {
            // We are passing in a variable
            llvm::Value *dumb2 = symbolTable->resolveSymbol(((IDNode *) node->getExprNodes()->at(i))->getID())->getPtr();
            dumb.push_back(dumb2);
        }
    }

    return ir->CreateCall(func, dumb);
}

llvm::Value *CodeGenerator::visit(CastExprNode *node) {
    llvm::Value *expr = visit(node->getExpr());

    llvm::Type *type = symbolTable->resolveType(node->getTypeString())->getTypeDef();

    return ct->varCast(type, expr);
}

llvm::Value *CodeGenerator::visit(ContinueNode *node) {
    if(whileStack->top()){
        ir->CreateBr(whileStack->top()->getStartWhileBB());
    }
    else {
        std::cout << "Continue statement does not reside in while loop\n";
    }
    return nullptr;
}

llvm::Value *CodeGenerator::visit(BreakNode *node) {
    if(whileStack->top()){
        ir->CreateBr(whileStack->top()->getMergeBB());
    }
    else {
        std::cout << "Continue statement does not reside in while loop\n";
    }
    return nullptr;
}
