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

std::vector<llvm::Value *> CodeGenerator::getParamVec(std::vector<ASTNode *> *paramNode,std::vector<ASTNode *> *arguNode ) {
    std::vector<llvm::Value *> paramVector;
    std::vector<std::string> aliasVector;

    llvm::Value* ptr;
    llvm::Value* val;
    ParamNode *pNode;
    Symbol * idNode;
    bool constant;
    llvm::Value *idx;
    std::string uniqueIden;
    llvm::Value *newParamPtr;

    for (unsigned long i = 0; i < arguNode->size(); ++i) {
        pNode = dynamic_cast<ParamNode *>(paramNode->at(i));
        llvm::Type * arguType;
        llvm::Type * paramType;
        llvm::Value * argPtr;
        bool c = true;
        //val = visit(arguNode->at(i));
        constant = pNode->isIsVar();

        if (pNode->getTupleType()) {
            //paramType = parseStructType(pNode->getTupleType());
            paramType = symbolTable->resolveType(pNode->getDeclaredType())->getTypeDef();
            auto argNode = dynamic_cast<TupleNode *>(arguNode->at(i));

            if (argNode) {
                argPtr = visit(argNode);
                assert(paramType->getStructNumElements() == (unsigned int)argNode->getElements()->size());
            } else {
                auto dumb = dynamic_cast<IDNode *>(arguNode->at(i));
                Symbol *symbol = symbolTable->resolveSymbol(dumb->getID());
                assert(symbol->getPtr()->getType()->getPointerElementType()->getStructNumElements()
                       == paramType->getStructNumElements());
                argPtr = symbol->getPtr();
                c = symbol->isConstant();
            }


            if(argNode || constant) {
                assert(constant);
                newParamPtr = ir->CreateAlloca(paramType);
                newParamPtr = it->initTuple(newParamPtr, it->getValueVectorFromStruct(argPtr));
                paramVector.push_back(newParamPtr);
                continue;
            } else if(!constant) {
                assert(!c);
                newParamPtr = ir->CreateAlloca(paramType);
                newParamPtr = it->initTupleFromPtrs(newParamPtr, it->getPtrVectorFromStruct(argPtr));
                paramVector.push_back(newParamPtr);
                continue;
            }


        } else {
            paramType = symbolTable->resolveType(pNode->getDeclaredType())->getTypeDef();
        }
        val = visit(arguNode->at(i));

        if (dynamic_cast<IndexTupleNode *>(arguNode->at(i))) {
            auto tupNode = dynamic_cast<IndexTupleNode *>(arguNode->at(i));

            if(dynamic_cast<INTNode *>(tupNode->getIndex())) {
                uniqueIden = std::to_string(((INTNode *) tupNode->getIndex())->value);
            } else {
                uniqueIden = ((IDNode *) tupNode->getIndex())->getID();
            }

            idNode = symbolTable->resolveSymbol(tupNode->getIdNode()->getID());
            idx = getIndexForTuple(tupNode->getIndex(), idNode->getPtr());
            ptr = getPtrToVar(idNode, constant, aliasVector, idx, uniqueIden);

        } else if (dynamic_cast<IDNode *> (arguNode->at(i))){
            idNode = symbolTable->resolveSymbol(((IDNode *) arguNode->at(i))->getID());
            ptr = getPtrToVar(idNode, constant, aliasVector);
        } else if ((val->getType() == charTy) || (val->getType() == intTy) || (val->getType() == realTy)){
            ptr = ir->CreateAlloca(val->getType());
            ir->CreateStore(val, ptr);
        } else {
            std::cerr << "Unknown type of var passed in\nAborting..\n";
            exit(1);
        }

        arguType = ptr->getType()->getPointerElementType();
        if((paramType != arguType) && (paramType == realTy)&& (arguType == intTy)) {
            llvm::Value *element = ir->CreateLoad(ptr);
            llvm::Value *newP = ir->CreateAlloca(realTy);
            element = ir->CreateSIToFP(element, realTy);
            ir->CreateStore(element, newP);
            ptr = newP;
        }

        paramVector.push_back(ptr);
    }

    return paramVector;
}

llvm::Value *CodeGenerator::getPtrToVar(Symbol *idNode, bool constant, std::vector<std::string> &aliasVector,
        llvm::Value *idxVal, std::string idxTrueVal) {
    llvm::Value *ptr, *val;
    std::string name;

    if (!constant) {
        assert(!idNode->isConstant());
        if (idxVal){
            ptr = it->getPtrFromTuple(idNode->getPtr(),idxVal);
            name = idNode->getName() + idxTrueVal;
        } else {
            ptr = idNode->getPtr();
            name = idNode->getName();
        }

        if (std::find(aliasVector.begin(), aliasVector.end(), "c"+name) != aliasVector.end()
            || std::find(aliasVector.begin(), aliasVector.end(), name) != aliasVector.end()){
            std::cerr << "Aliasing error\nAborting...\n";
            exit(1);
        }
        aliasVector.push_back(idNode->getName());
        return ptr;
    } else {
        if (idxVal) {
            val = it->getValFromTuple(idNode->getPtr(),idxVal);
            ptr = ir->CreateAlloca(val->getType());
            name = idNode->getName() + idxTrueVal;
        } else {
            ptr = ir->CreateAlloca(idNode->getPtr()->getType()->getPointerElementType());
            val = ir->CreateLoad(idNode->getPtr());
            name = idNode->getName();
        }
        ir->CreateStore(val, ptr);

        if (std::find(aliasVector.begin(), aliasVector.end(), name) != aliasVector.end()){
            std::cerr << "Aliasing error\nAborting...\n";
            exit(1);
        }
        aliasVector.push_back("c" + idNode->getName());
        return ptr;
    }
}

llvm::StructType *CodeGenerator::parseStructType(TupleType *node) {
    auto *declNodes     = node->getDecls();
    auto *members       = new std::vector<llvm::Type *>;
    auto *memberNames   = new std::unordered_map<std::string, int>;
    int   i             = 0;
    std::string tupleID = "tuple";

    llvm::StructType* newStruct;
    llvm::Type*       type;
    std::string       typeStr;
    std::string       memberID;

    for (auto element : * declNodes) {
        typeStr = ((DeclNode *) element)->getTypeIds()->at(0);
        type    = symbolTable->resolveType(typeStr)->getTypeDef();
        members->push_back(type->getPointerTo());

        tupleID += typeStr;
        memberID = ((DeclNode *) element)->getID();
        if(not(memberID.empty())) {
            memberNames->insert(std::pair<std::string, int>(memberID, i));
            tupleID += "|" + memberID;
        }
        ++i;
    }

    newStruct = llvm::StructType::create(*members, tupleID);
    symbolTable->addTupleType(newStruct, memberNames, members); //this is where we add the struct to the symbol table
    return newStruct;
}

llvm::Value *CodeGenerator::getIndexForTuple(ASTNode *index, llvm::Value *tuplePtr) {
    llvm::Value *idx;
    if(dynamic_cast<IDNode *>(index)){
        llvm::Type * type = tuplePtr->getType()->getPointerElementType();
        assert(type->isStructTy());
        auto *structType = llvm::cast<llvm::StructType>(type);
        GazpreaTupleType *gtt = symbolTable->resolveTupleType(structType);
        int i = gtt->getOffsetFromString(dynamic_cast<IDNode *>(index)->getID());
        idx = it->getConsi32(i);
    }
    else{
        idx = visit(index);
    }

    return idx;
}

llvm::Function* CodeGenerator::declareFuncOrProc(std::string functionName, std::string strRetType, std::vector<ASTNode *>
        *paramsList, int nodeType, int line, TupleType *tupleType) {
    std::vector<llvm::Type *> params;
    llvm::Type               *retType;
    llvm::FunctionType       *funcTy;
    llvm::Function           *F;
    llvm::StructType         *structType;
    std::string               typeName;

    /*
     * Setting Return Type
     *
     * Case 1: Return type is tuple and it has not been added to the table yet
     * Case 2: Return type is tuple and it was added to the symbol table
     * Case 3: Return type is either global or a base type
     */
    if ((tupleType) && (not(symbolTable->resolveType(strRetType)))) {
        retType = parseStructType(tupleType);
        GazpreaTupleType *garb = symbolTable->resolveTupleType(retType);
        symbolTable->addTupleType(strRetType, retType, garb->getStringRefMap(), garb->getMembers());
    } else if (tupleType) {
        GazpreaType * gazpreaType = symbolTable->resolveType(strRetType);
        retType = llvm::cast<llvm::StructType>(gazpreaType->getTypeDef());
    } else {
        retType = symbolTable->resolveType(strRetType)->getTypeDef();
    }

    /*
     * Setting parameter type
     *
     * Similar process to above, we just do it for each parameter
     */
    for (auto it = paramsList->begin(); it!= paramsList->end(); ++it) {
        //var decl
        auto * pNode = (ParamNode *) it.operator*();
        typeName = (pNode)->getDeclaredType();

        if(pNode->getTupleType() && not(symbolTable->resolveType(typeName))){
            structType = parseStructType(pNode->getTupleType());
            GazpreaTupleType * garb = symbolTable->resolveTupleType(structType);
            symbolTable->addTupleType(typeName, structType, garb->getStringRefMap(),  garb->getMembers());

            params.push_back(structType->getPointerTo());
        } else if(pNode->getTupleType()) {
            GazpreaType * gazpreaType = symbolTable->resolveType(typeName);
            structType = llvm::cast<llvm::StructType>(gazpreaType->getTypeDef());

            params.push_back(structType->getPointerTo());
        } else {
            params.push_back(symbolTable->resolveType(typeName)->getTypeDef()->getPointerTo());
        }
    }

    symbolTable->addFunctionSymbol(functionName, nodeType, paramsList);

    if(tupleType){
        funcTy = llvm::FunctionType::get(retType->getPointerTo(), params, false);
    } else {
        funcTy = llvm::FunctionType::get(retType, params, false);
    }

    F = llvm::Function::Create(funcTy, llvm::Function::ExternalLinkage, functionName, mod);

    //handle errors
    if (F->getName() != functionName) {
        F->eraseFromParent();
        F = mod->getFunction(functionName);

        if (!F->empty()) {
            std::cerr << "redefinition of function at line " << line << ". Aborting...\n";
            exit(1);
        }

        if(F->arg_size() != paramsList->size()) {
            std::cerr << "redefinition of function with different # args at line " << line << ". Aborting...\n";
            exit(1);
        }
    }
    return F;
}

void CodeGenerator::generateFuncOrProcBody(llvm::Function *F, std::vector<ASTNode *> *paramsList, ASTNode * block) {
    //new scope
    symbolTable->pushNewScope();

    //preserve old while stack
    auto *oldWhileStack = whileStack;
    whileStack = new std::stack<WhileBuilder *>;

    size_t idx = 0;
    for (auto AI = F->arg_begin(); idx != F->arg_size(); ++AI, ++idx){
        auto *p = (ParamNode *) paramsList->at(idx);
        symbolTable->addSymbol(p->getVarName(), p->getType(), false, AI);
        AI->setName(p->getVarName());
    }

    // Create an entry block and set the inserter.
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(*globalCtx, "entry", F);
    ir->SetInsertPoint(entry);

    ///*//kyle's testing code
    printf("running kyles print test\n");
    llvm::Value * vec = et->getNewVector(it->getConsi32(INTEGER));
    et->initVector(vec, it->getConsi32(5));
    et->setIdentityVector(vec);
    et->printVector(vec);
    ir->CreatePointerCast(vec, intVecTy->getPointerTo());
    ir->CreatePointerCast(vec, boolVecTy->getPointerTo());
    ir->CreatePointerCast(vec, charVecTy->getPointerTo());
    ir->CreatePointerCast(vec, realVecTy->getPointerTo());
    //*/

    //visit block and create ir
    visit(block);

    //pop scope, swap back in while stack
    symbolTable->popScope();
    whileStack = oldWhileStack;
}

llvm::Value *CodeGenerator::callFuncOrProc(std::string functionName, std::vector<ASTNode *> *arguments){
    //get function
    FunctionSymbol *functionSymbol = (FunctionSymbol *) symbolTable->resolveSymbol(functionName);
    llvm::Function *func = mod->getFunction(functionName);

    //get params
    std::vector<llvm::Value *> paramVec = getParamVec(functionSymbol->getParamsVec(), arguments);

    return ir->CreateCall(func, paramVec);
}
