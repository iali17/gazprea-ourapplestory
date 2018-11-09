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
    std::vector<llvm::Value *> paramVector = getParamVec(functionSymbol->getParamsVec(), node->getExprNodes());

    return ir->CreateCall(func, paramVector);
}

llvm::Value *CodeGenerator::visit(ProcedureNode *node) {

    std::vector<ASTNode *>  paramsList = *node->getParamNodes();
    std::vector<llvm::Type *> params;

    llvm::Type *retType;
    if (node->getRetType().substr(0,6) == "tuple("){
        if(not(symbolTable->resolveType(node->getRetType()))){
            retType = parseStructType(node->getTupleType());
            GazpreaTupleType * garb = symbolTable->resolveTupleType(retType);
            symbolTable->addTupleType(node->getRetType(), retType, garb->getStringRefMap(),  garb->getMembers());
        } else {
            GazpreaType * gazpreaType = symbolTable->resolveType(node->getRetType());
            retType = llvm::cast<llvm::StructType>(gazpreaType->getTypeDef());
        }
    } else {
        retType = symbolTable->resolveType(node->getRetType())->getTypeDef();
    }

    for (auto it = paramsList.begin(); it!= paramsList.end(); ++it) {
        auto * pNode = (ParamNode *) it.operator*();
        std::string typeName = (pNode)->getDeclaredType();
        if(pNode->getTupleType()){
            llvm::StructType * structType;
            if(not(symbolTable->resolveType(typeName))){
                structType = parseStructType(pNode->getTupleType());
                GazpreaTupleType * garb = symbolTable->resolveTupleType(structType);
                symbolTable->addTupleType(typeName, structType, garb->getStringRefMap(),  garb->getMembers());
            } else {
                GazpreaType * gazpreaType = symbolTable->resolveType(typeName);
                structType = llvm::cast<llvm::StructType>(gazpreaType->getTypeDef());
            }
            params.push_back(structType->getPointerTo());
        } else {
            params.push_back(symbolTable->resolveType(typeName)->getTypeDef()->getPointerTo());
        }
    }

    symbolTable->addFunctionSymbol(node->getProcedureName(), node->getType(), node->getParamNodes());
    llvm::FunctionType *funcTy;
    if(node->getTupleType()){
        funcTy = llvm::FunctionType::get(retType->getPointerTo(), params, false);
    }else {
        funcTy = llvm::FunctionType::get(retType, params, false);
    }

    llvm::Function *F = llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage, node->getProcedureName(), mod);

    if (F->getName() != node->getProcedureName()) {
        F->eraseFromParent();
        F = mod->getFunction(node->getProcedureName());

        if (!F->empty()) {
            std::cerr << "redefinition of function at line " << node->getLine() << ". Aborting...\n";
            exit(1);
        }

        if(F->arg_size() != paramsList.size()) {
            std::cerr << "redefinition of function with different # args at line "<< node->getLine() << ". Aborting...\n";
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

    if(!F->getBasicBlockList().back().getTerminator()){
        if(F->getReturnType() == llvm::Type::getVoidTy(*globalCtx)){
            ir->CreateRetVoid();
        } else if (F->getBasicBlockList().back().empty()) {
            F->getBasicBlockList().back().eraseFromParent();
        } else {
            std::cerr << "Missing return statement in function defined at line " << node->getLine() << ". Aborting...\n";
            exit(1);
        }
    }

    return nullptr;
}

llvm::Value *CodeGenerator::visit(FunctionNode *node) {
    std::vector<ASTNode *>  paramsList = *node->getParamNodes();
    std::vector<llvm::Type *> params;

    llvm::Type *retType;

    if (node->getRetType().substr(0,6) == "tuple("){
        if(not(symbolTable->resolveType(node->getRetType()))){
            retType = parseStructType(node->getTupleType());
            GazpreaTupleType * garb = symbolTable->resolveTupleType(retType);
            symbolTable->addTupleType(node->getRetType(), retType, garb->getStringRefMap(),  garb->getMembers());
        } else {
            GazpreaType * gazpreaType = symbolTable->resolveType(node->getRetType());
            retType = llvm::cast<llvm::StructType>(gazpreaType->getTypeDef());
        }
    } else {
        retType = symbolTable->resolveType(node->getRetType())->getTypeDef();
    }

    for (auto it = paramsList.begin(); it!= paramsList.end(); ++it) {
        auto * pNode = (ParamNode *) it.operator*();
        std::string typeName = (pNode)->getDeclaredType();
        assert(pNode->isIsVar());
        if(pNode->getTupleType()){
            llvm::StructType * structType;
            if(not(symbolTable->resolveType(typeName))){
                structType = parseStructType(pNode->getTupleType());
                GazpreaTupleType * garb = symbolTable->resolveTupleType(structType);
                symbolTable->addTupleType(typeName, structType, garb->getStringRefMap(),  garb->getMembers());
            } else {
                GazpreaType * gazpreaType = symbolTable->resolveType(typeName);
                structType = llvm::cast<llvm::StructType>(gazpreaType->getTypeDef());
            }
            params.push_back(structType->getPointerTo());
        } else {
            params.push_back(symbolTable->resolveType(typeName)->getTypeDef()->getPointerTo());
        }
    }

    symbolTable->addFunctionSymbol(node->getFunctionName(), node->getType(), node->getParamNodes());
    llvm::FunctionType *funcTy;
    if(node->getTupleType()){
        funcTy = llvm::FunctionType::get(retType->getPointerTo(), params, false);
    }else {
        funcTy = llvm::FunctionType::get(retType, params, false);
    }


    llvm::Function *F = llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage, node->getFunctionName(), mod);

    if (F->getName() != node->getFunctionName()) {
        F->eraseFromParent();
        F = mod->getFunction(node->getFunctionName());

        if (!F->empty()) {
            std::cerr << "redefinition of function at line " << node->getLine() << ". Aborting...\n";
            exit(1);
        }

        if(F->arg_size() != paramsList.size()) {
            std::cerr << "redefinition of function with different # args at line " << node->getLine() << ". Aborting...\n";
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
    //TODO THIS DOES NOT ENFORCE FLOW BUT YEaH
/*
    if(!F->getBasicBlockList().back().getTerminator()){
        std::cerr << "Missing return statement in function defined at line: " << node->getLine() << '\n';
        exit(13);
    }*/
    if (F->getBasicBlockList().back().empty()) {
        F->getBasicBlockList().back().eraseFromParent();
    }

    return nullptr;
}

llvm::Value* CodeGenerator::visit(ProcedureCallNode *node) {
    FunctionSymbol *functionSymbol = (FunctionSymbol *) symbolTable->resolveSymbol(node->getProcedureName());
    llvm::Function *func = mod->getFunction(node->getProcedureName());
    std::vector<llvm::Value *> dumb = getParamVec(functionSymbol->getParamsVec(), node->getExprNode());

    llvm::Value *val = ir->CreateCall(func, dumb);


    if (node->getUnOp() == NEG) {
        if (val->getType() == realTy){
            val  =  ir->CreateFNeg(val, "fnegtmp");
        } else {
            val = ir->CreateNeg(val, "negtmp");
        }
    } else if (node->getUnOp() == MINUS) {
        if(val->getType() == intTy){
            val =  ir->CreateSub(it->getConsi32(0), val, "iaddtmp");
        }
        else if(val->getType() == realTy){
            val =  ir->CreateFSub(it->getConsi32(0), val, "faddtmp");
        }
    }

    llvm::Value* ptr = nullptr;

    if (node->getTypeIds()->size() == 0){
        if(it->isStructType(val)){
            ptr = val;
            symbolTable->addSymbol(node->getVarName(), node->getType(), true);

        } else {
            ptr = ir->CreateAlloca(val->getType());
            ir->CreateStore(val, ptr);
            symbolTable->addSymbol(node->getVarName(), node->getType(), true);
        }

    } else if (node->getTypeIds()->size() == 1){
        if(it->isStructType(val)){
            //ptr = val;
            Symbol * symbol = symbolTable->resolveSymbol(node->getVarName());
            llvm::StructType *structType = parseStructType(node->getTupleType());
            ptr = ir->CreateAlloca(structType);
            ptr = it->initTuple(ptr, it->getValueVectorFromStruct(val));
            symbolTable->addSymbol(node->getVarName(), node->getType(), true);

        } else {
            llvm::Type *type = symbolTable->resolveType(node->getTypeIds()->at(0))->getTypeDef();
            ptr = ir->CreateAlloca(type);
            if (val == nullptr) {
                if (!(it->setNull(type, ptr))) {
                      std::cerr << "Unable to initialize to null at line " << node->getLine() << ". Aborting...\n";
                      exit(1);
                }
                symbolTable->addSymbol(node->getVarName(), node->getType(), node->isConstant(), ptr);
                return nullptr;
            }
            ir->CreateStore(val, ptr);
            symbolTable->addSymbol(node->getVarName(), node->getType(), node->isConstant());
        }
    }

    symbolTable->resolveSymbol(node->getVarName())->setPtr(ptr);

    return nullptr;
}

llvm::Value *CodeGenerator::visit(ParamNode *node) {
    return nullptr;
}

llvm::Value *CodeGenerator::visit(ProtoProcedureNode * node) {
    std::vector<ASTNode *>  paramsList = *node->getParamNodes();
    std::vector<llvm::Type *> params;

    llvm::Type *retType;

    if (node->getRetType().substr(0,6) == "tuple("){
        if(not(symbolTable->resolveType(node->getRetType()))){
            retType = parseStructType(node->getTupleType());
            GazpreaTupleType * garb = symbolTable->resolveTupleType(retType);
            symbolTable->addTupleType(node->getRetType(), retType, garb->getStringRefMap(),  garb->getMembers());
        } else {
            GazpreaType * gazpreaType = symbolTable->resolveType(node->getRetType());
            retType = llvm::cast<llvm::StructType>(gazpreaType->getTypeDef());
        }
    } else {
        retType = symbolTable->resolveType(node->getRetType())->getTypeDef();
    }

    for (auto it = paramsList.begin(); it!= paramsList.end(); ++it) {
        auto * pNode = (ParamNode *) it.operator*();
        std::string typeName = (pNode)->getDeclaredType();
        if(pNode->getTupleType()){
            llvm::StructType * structType;
            if(not(symbolTable->resolveType(typeName))){
                structType = parseStructType(pNode->getTupleType());
                GazpreaTupleType * garb = symbolTable->resolveTupleType(structType);
                symbolTable->addTupleType(typeName, structType, garb->getStringRefMap(),  garb->getMembers());
            } else {
                GazpreaType * gazpreaType = symbolTable->resolveType(typeName);
                structType = llvm::cast<llvm::StructType>(gazpreaType->getTypeDef());
            }
            params.push_back(structType->getPointerTo());
        } else {
            params.push_back(symbolTable->resolveType(typeName)->getTypeDef()->getPointerTo());
        }
    }

    symbolTable->addFunctionSymbol(node->getProcedureName(), node->getType(), node->getParamNodes());
    llvm::FunctionType *funcTy;
    if(node->getTupleType()){
        funcTy = llvm::FunctionType::get(retType->getPointerTo(), params, false);
    }else {
        funcTy = llvm::FunctionType::get(retType, params, false);
    }

    llvm::Function *F = llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage, node->getProcedureName(), mod);

    if (F->getName() != node->getProcedureName()) {
        F->eraseFromParent();
        F = mod->getFunction(node->getProcedureName());

        if (!F->empty()) {
            std::cerr << "redefinition of function at line " << node->getLine() << ". Aborting...\n";
            exit(1);
        }

        if(F->arg_size() != paramsList.size()) {
            std::cerr << "redefinition of function with different # args at line " << node->getLine() << ". Aborting...\n";
            exit(1);
        }
    }
    return nullptr;
}