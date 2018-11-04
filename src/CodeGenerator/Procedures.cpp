//
// Created by iali1 on 11/4/18.
//

#include <CodeGenerator/CodeGenerator.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;

llvm::Value *CodeGenerator::visit(CallNode *node) {
    FunctionSymbol *functionSymbol = (FunctionSymbol *) symbolTable->resolveSymbol(node->getProcedureName());
    llvm::Function *func = mod->getFunction(node->getProcedureName());
    std::vector<llvm::Value *> dumb = getParamVec(functionSymbol->getParamsVec(), node->getExprNodes());

    return ir->CreateCall(func, dumb);
}

llvm::Value *CodeGenerator::visit(ProcedureNode *node) {

    std::vector<ASTNode *>  paramsList = *node->getParamNodes();
    std::vector<llvm::Type *> params;

    llvm::Type *retType = symbolTable->resolveType(node->getRetType())->getTypeDef();

    for (auto it = paramsList.begin(); it!= paramsList.end(); ++it) {
        std::string typeName =  ((ParamNode *) it.operator*())->getDeclaredType();

        params.push_back(symbolTable->resolveType(typeName)->getTypeDef()->getPointerTo());
    }

    symbolTable->addFunctionSymbol(node->getProcedureName(), node->getType(), node->getParamNodes());
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

    if(!F->getEntryBlock().getTerminator()){
        ir->CreateRetVoid();
    }

    return nullptr;
}

llvm::Value* CodeGenerator::visit(ProcedureCallNode *node) {
    FunctionSymbol *functionSymbol = (FunctionSymbol *) symbolTable->resolveSymbol(node->getProcedureName());
    llvm::Function *func = mod->getFunction(node->getProcedureName());
    std::vector<llvm::Value *> dumb = getParamVec(functionSymbol->getParamsVec(), node->getExprNode());

    llvm::Value *val = ir->CreateCall(func, dumb);
    llvm::Value* ptr = nullptr;

    if (node->getTypeIds()->size() == 0){
        ptr = ir->CreateAlloca(val->getType());
        ir->CreateStore(val, ptr);
        symbolTable->addSymbol(node->getVarName(), node->getType(), true);
    } else if (node->getTypeIds()->size() == 1){
        llvm::Type *type = symbolTable->resolveType(node->getTypeIds()->at(0))->getTypeDef();
        ptr = ir->CreateAlloca(type);
        if(val == nullptr) {
            if (!(it->setNull(type, ptr))){
                std::cerr << "Unable to initialize to null\n";
            }
            symbolTable->addSymbol(node->getVarName(), node->getType(), node->isConstant(), ptr);
            return nullptr;
        }
        ir->CreateStore(val, ptr);
        symbolTable->addSymbol(node->getVarName(), node->getType(), node->isConstant());
    }

    symbolTable->resolveSymbol(node->getVarName())->setPtr(ptr);

    return nullptr;
}

llvm::Value *CodeGenerator::visit(ParamNode *node) {
    return nullptr;
}