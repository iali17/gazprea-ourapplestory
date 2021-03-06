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

/**
 * Checks if paramater and arguments are the same, if they are set parameter
 * else error
 *
 * @param paramNode
 * @param arguNode
 * @return std::vector<llvm::Value *>
 */
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
        constant = pNode->isIsVar();

        if (pNode->getTupleType()) {
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
                newParamPtr = initTuple(newParamPtr, it->getValueVectorFromTuple(argPtr));
                paramVector.push_back(newParamPtr);
                continue;
            } else if(!constant) {
                assert(!c);
                newParamPtr = ir->CreateAlloca(paramType);
                newParamPtr = it->initTupleFromPtrs(newParamPtr, it->getPtrVectorFromStruct(argPtr));
                paramVector.push_back(newParamPtr);
                continue;
            }


        } else if (pNode->getGType() == VECTOR_T) {
            auto nameVec = split(pNode->getDeclaredType(), '[');
            std::string typeName = nameVec[0];
            std::string sizeName = nameVec[1];
            sizeName.erase(std::remove(sizeName.begin(), sizeName.end(), ']'), sizeName.end());
            int size = std::stoi(sizeName);
            paramType = it->getDeclVectorType(typeName);

            auto argNode = dynamic_cast<VectorNode *>(arguNode->at(i));
            argPtr = visit(arguNode->at(i));

            if (argNode) {
                argPtr = visit(argNode);

                llvm::Value *vec = nullptr;
                vec = et->getNewVector(it->getConstFromType(argPtr->getType()));
                et->initVector(vec,it->getConsi32(size));
                et->strictCopyVectorElements(vec, argPtr, it->getConsi32(pNode->getLine()), it->getConsi32(0));

            } else if (dynamic_cast<IndexTupleNode *>(arguNode->at(i))) {
                auto tupNode = dynamic_cast<IndexTupleNode *>(arguNode->at(i));

                if(dynamic_cast<INTNode *>(tupNode->getIndex())) {
                    uniqueIden = std::to_string(((INTNode *) tupNode->getIndex())->value);
                } else {
                    uniqueIden = ((IDNode *) tupNode->getIndex())->getID();
                }

                idNode = symbolTable->resolveSymbol(tupNode->getIdNode()->getID());
                idx = getIndexForTuple(tupNode->getIndex(), idNode->getPtr());
                c = idNode->isConstant();
                argPtr = getPtrToVar(idNode, constant, aliasVector, idx, uniqueIden);
                argPtr = ir->CreateLoad(argPtr);

            } else if(dynamic_cast<IDNode *>(arguNode->at(i))) {
                auto dumb = dynamic_cast<IDNode *>(arguNode->at(i));
                Symbol *symbol = symbolTable->resolveSymbol(dumb->getID());
                argPtr = symbol->getPtr();
                c = symbol->isConstant();
            } else {
                llvm::Value *vec = nullptr;
                vec = et->getNewVector(it->getConstFromType(argPtr->getType()));
                et->initVector(vec,it->getConsi32(size));
                et->strictCopyVectorElements(vec, argPtr, it->getConsi32(pNode->getLine()), it->getConsi32(0));
            }

            if (argNode || constant) {
                assert(constant);
                newParamPtr = et->getVectorCopy(argPtr);
                paramVector.push_back(newParamPtr);
                continue;
            } else if (!constant) {
                assert(!c);
                paramVector.push_back(argPtr);
                continue;
            }

        } else if (pNode->getGType() == MATRIX) {
                auto nameVec = split(pNode->getDeclaredType(), '[');
                std::string typeName = nameVec[0];
                std::string sizeName = nameVec[1];
                sizeName.erase(std::remove(sizeName.begin(), sizeName.end(), ']'), sizeName.end());
                auto sizes = split(sizeName, ',');
                int leftSize = std::stoi(sizes[0]);
                int rightSize = std::stoi(sizes[1]);
                paramType = it->getDeclMatrixType(typeName);

                auto argNode = dynamic_cast<MatrixNode *>(arguNode->at(i));
                argPtr = visit(arguNode->at(i));
                if (argNode) {
                    argPtr = visit(argNode);
                    llvm::Value *mat = nullptr;
                    mat = et->getNewMatrix(it->getConstFromType(argPtr->getType()));
                    et->initMatrix(mat,it->getConsi32(leftSize),it->getConsi32(rightSize));
                    et->strictCopyMatrixElements(mat, argPtr, it->getConsi32(pNode->getLine()), it->getConsi32(0));

                } else if (dynamic_cast<IndexTupleNode *>(arguNode->at(i))) {
                    auto tupNode = dynamic_cast<IndexTupleNode *>(arguNode->at(i));

                    if(dynamic_cast<INTNode *>(tupNode->getIndex())) {
                        uniqueIden = std::to_string(((INTNode *) tupNode->getIndex())->value);
                    } else {
                        uniqueIden = ((IDNode *) tupNode->getIndex())->getID();
                    }

                    idNode = symbolTable->resolveSymbol(tupNode->getIdNode()->getID());
                    c = idNode->isConstant();
                    idx = getIndexForTuple(tupNode->getIndex(), idNode->getPtr());
                    argPtr = getPtrToVar(idNode, constant, aliasVector, idx, uniqueIden);
                    argPtr = ir->CreateLoad(argPtr);

                } else if(dynamic_cast<IDNode *>(arguNode->at(i))) {
                    auto dumb = dynamic_cast<IDNode *>(arguNode->at(i));
                    Symbol *symbol = symbolTable->resolveSymbol(dumb->getID());
                    argPtr = symbol->getPtr();
                    c = symbol->isConstant();
                } else {
                    llvm::Value *mat = nullptr;
                    mat = et->getNewMatrix(it->getConstFromType(argPtr->getType()));
                    et->initMatrix(mat, it->getConsi32(leftSize), it->getConsi32(rightSize));
                    et->strictCopyMatrixElements(mat, argPtr, it->getConsi32(pNode->getLine()), it->getConsi32(0));
                }

                if(argNode || constant) {
                    assert(constant);
                    newParamPtr = et->getMatrixCopy(argPtr);
                    paramVector.push_back(newParamPtr);
                    continue;
                } else if(!constant) {
                    assert(!c);
                    paramVector.push_back(argPtr);
                    continue;
                }
        } else if (pNode->getGType() == INTERVAL) {
            paramType = intervalTy;

            auto argNode = dynamic_cast<IntervalNode *>(arguNode->at(i));
            if (argNode) {
                argPtr = visit(argNode);
            } else if (dynamic_cast<IndexTupleNode *>(arguNode->at(i))) {
                auto tupNode = dynamic_cast<IndexTupleNode *>(arguNode->at(i));

                if (dynamic_cast<INTNode *>(tupNode->getIndex())) {
                    uniqueIden = std::to_string(((INTNode *) tupNode->getIndex())->value);
                } else {
                    uniqueIden = ((IDNode *) tupNode->getIndex())->getID();
                }

                idNode = symbolTable->resolveSymbol(tupNode->getIdNode()->getID());
                idx = getIndexForTuple(tupNode->getIndex(), idNode->getPtr());
                c = idNode->isConstant();
                argPtr = getPtrToVar(idNode, constant, aliasVector, idx, uniqueIden);
                argPtr = ir->CreateLoad(argPtr);
            } else {
                auto dumb = dynamic_cast<IDNode *>(arguNode->at(i));
                Symbol *symbol = symbolTable->resolveSymbol(dumb->getID());
                argPtr = symbol->getPtr();
                c = symbol->isConstant();
            }

            if (argNode || constant) {
                assert(constant);
                paramVector.push_back(argPtr);
                continue;
            } else if (!constant) {
                assert(!c);
                paramVector.push_back(argPtr);
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

/**
 * Handles aliasing and get tuple members from tuples
 *
 * @param idNode
 * @param constant
 * @param aliasVector
 * @param idxVal
 * @param idxTrueVal
 * @return llvm::Value *
 */
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

/**
 * Deals with string parsing and finding the tuple members
 *
 * @param node
 * @return llvm::StructType *
 */
llvm::StructType *CodeGenerator::parseStructType(TupleTypeNode *node) {
    auto *declNodes     = node->getDecls();
    auto *members       = new std::vector<llvm::Type *>;
    auto *memberNames   = new std::unordered_map<std::string, int>;
    int   i             = 0;
    std::string tupleID = "tuple";

    llvm::StructType* newStruct;
    llvm::Type*       type;
    std::string       typeStr;
    std::string       memberID;
    InternalTools::tupleGarbo tg;
    auto * dims = new std::unordered_map<int, std::pair<int, int>>;
    GazpreaType * gazpreaType;

    for (auto element : * declNodes) {
        typeStr = ((DeclNode *) element)->getTypeIds()->at(0);
        gazpreaType = symbolTable->resolveType(typeStr);


        if(gazpreaType == nullptr){
            tg = it->parseStringExtension(typeStr);
            type = tg.type->getPointerTo();
            auto  p = std::make_pair(tg.leftIndex, tg.rightIndex);
            dims->insert(std::make_pair(i, p));
        } else {
            type    = gazpreaType->getTypeDef();
        }

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
    symbolTable->addTupleType(newStruct, memberNames, members, dims); //this is where we add the struct to the symbol table
    return newStruct;
}

/**
 * Handles getting the elem at the index position given the index number or the parameter name if applicable
 *
 * @param index
 * @param tuplePtr
 * @return llvm::Value *
 */
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

/**
 * Makes the function in ir
 *
 * @param functionName
 * @param strRetType
 * @param paramsList
 * @param nodeType
 * @param line
 * @param tupleType
 * @param gType
 * @return llvm::Function *
 */
llvm::Function* CodeGenerator::declareFuncOrProc(std::string functionName, std::string strRetType, std::vector<ASTNode *>
        *paramsList, int nodeType, int line, TupleTypeNode *tupleType, int gType) {
    std::vector<llvm::Type *> params;
    llvm::Type               *retType;
    llvm::FunctionType       *funcTy;
    llvm::Function           *F;
    llvm::StructType         *structType;
    std::string               typeName;
    int                       sizeLeft = -1;
    int                       sizeRight  = -1;
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
    } else if (gType == VECTOR_T) {
        auto nameSize = split(strRetType, '[');
        auto sizeName = nameSize[1];
        sizeName.erase(std::remove(sizeName.begin(), sizeName.end(), ']'), sizeName.end());
        sizeLeft = std::stoi(sizeName);
        strRetType = nameSize[0];
        auto rename = split(strRetType, 'v');
        strRetType = rename[0] + "vector";
        retType = it->getDeclVectorType(strRetType);
    } else if (gType == MATRIX) {
        auto nameSize = split(strRetType, '[');
        auto fullSize = nameSize[1];
        fullSize.erase(std::remove(fullSize.begin(), fullSize.end(), ']'), fullSize.end());
        auto sizes = split(fullSize, ',');
        sizeLeft = std::stoi(sizes[0]);
        sizeRight = std::stoi(sizes[1]);
        strRetType = nameSize[0];
        auto rename = split(strRetType, 'm');
        strRetType = rename[0] + "matrix";
        retType = it->getDeclMatrixType(strRetType);
    } else if (gType == INTERVAL){
       retType = intervalTy;
    } else {
        retType = symbolTable->resolveType(strRetType)->getTypeDef();
    }

    /*
     * Setting parameter type
     *
     * Similar process to above, we just do it for each parameter
     */
    for (auto ite = paramsList->begin(); ite!= paramsList->end(); ++ite) {
        //var decl
        auto * pNode = (ParamNode *) ite.operator*();
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
        } else if (pNode->getGType() == VECTOR_T) {
            auto nameVec = split(typeName, '[');
            typeName = nameVec[0];
            auto rename = split(typeName, 'v');
            typeName = rename[0] + "vector";
            params.push_back(it->getDeclVectorType(typeName)->getPointerTo());
        } else if (pNode->getGType() == MATRIX) {
            auto nameVec = split(typeName, '[');
            typeName = nameVec[0];
            auto rename = split(typeName, 'm');
            typeName = rename[0] + "matrix";
            params.push_back(it->getDeclMatrixType(typeName)->getPointerTo());
        } else if (pNode->getGType() == INTERVAL) {
            params.push_back(intervalTy->getPointerTo());
        } else {
            params.push_back(symbolTable->resolveType(typeName)->getTypeDef()->getPointerTo());
        }
    }

    symbolTable->addFunctionSymbol(functionName, nodeType, paramsList);

    if(tupleType || gType != -1){
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
    sad->insert(std::pair<llvm::Function *, std::pair<std::pair<int, int>, int>>(F, std::make_pair(std::make_pair(sizeLeft, sizeRight), line)));
    return F;
}

/**
 * Creates the function and procedure block then calls lines regularly in a new scope
 *
 * @param F
 * @param paramsList
 * @param block
 */
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

    //visit block and create ir
    visit(block);

    //pop scope, swap back in while stack
    symbolTable->popScope();
    whileStack = oldWhileStack;
}

/**
 * Handles function and procedure calls
 *
 * @param functionName
 * @param arguments
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::callFuncOrProc(std::string functionName, std::vector<ASTNode *> *arguments){
    //get function
    FunctionSymbol *functionSymbol = (FunctionSymbol *) symbolTable->resolveSymbol(functionName);
    llvm::Function *func = mod->getFunction(functionName);

    //get params
    std::vector<llvm::Value *> paramVec = getParamVec(functionSymbol->getParamsVec(), arguments);

    llvm::Value * retVal = ir->CreateCall(func, paramVec);

    auto iter = sad->find(func);

    // Enforcing return type is the same for vector and matrices
    if (iter == sad->end()) {
        return retVal;
    } else if (iter->second.first.first == -1 && iter->second.first.second == -1) {
        return retVal;
    } else if (iter->second.first.second == -1) {
        // This is just to test for type errors
        llvm::Value *realRetVal = et->getNewVector(it->getConstFromType(func->getReturnType()));
        et->initVector(realRetVal, it->getConsi32(iter->second.first.first));
        //realRetVal = it->castVectorToType(realRetVal,func->getReturnType());
        et->strictCopyVectorElements(realRetVal, retVal, it->getConsi32(iter->second.second), it->getConsi32(false));
    } else {
        // This is just to test for type errors
        llvm::Value *realRetVal = et->getNewMatrix(it->getConstFromType(func->getReturnType()));
        et->initMatrix(realRetVal, it->getConsi32(iter->second.first.first), it->getConsi32(iter->second.first.second));
        //realRetVal = it->castVectorToType(realRetVal,func->getReturnType());
        et->strictCopyMatrixElements(realRetVal, retVal, it->getConsi32(iter->second.second), it->getConsi32(0));
    }

    return retVal;
}

/**
 * Creates vector from an interval
 *
 * @param range
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::getRange(ASTNode *range) {
    //visit the range, do the implicit by if needed, point to the integer elements
    llvm::Value * rangeVecPtr = visit(range);
    if(rangeVecPtr->getType() == intervalTy->getPointerTo()){
        rangeVecPtr = et->getVectorFromInterval(rangeVecPtr, it->getConsi32(1));
    }
    return rangeVecPtr;
}

// from https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> CodeGenerator::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

/**
 * Set null of vector or matrix
 *
 * @param val
 */
void CodeGenerator::setNullVecOrMat(llvm::Value *val) {
    if(it->isMatrixType(val))
        et->setIdentityMatrix(val);
    else
        et->setIdentityVector(val);
}

/**
 * Set identity of vector or matrix
 *
 * @param val
 */
void CodeGenerator::setIdentityVecOrMat(llvm::Value *val) {
    if(it->isMatrixType(val))
        et->setIdentityMatrix(val);
    else
        et->setIdentityVector(val);
}

/**
 * Gets the vector inside the tuple
 *
 * @param node
 * @return
 */
llvm::Value *CodeGenerator::visit(IndexFilterNode *node) {
    llvm::Value * tup = visit(node->getFilterNode());
    return it->getValFromStruct(tup, node->getIndex());
}

/**
 * Frees memory when program ends
 *
 * @param ptrs
 */
void CodeGenerator::freeMem(std::vector<llvm::Value *> *ptrs) {
    for (uint i = 0; i < ptrs->size(); i++) {
        llvm::Value *ptr = ptrs->at(i);
        if(it->isIntervalType(ptr)){
            et->destroyInterval(ptr);
        }

        if(it->isMatrixType(ptr)){
            et->destroyMatrix(ptr);
        }

        if(it->isVectorType(ptr)){
            et->destroyVector(ptr);
        }

        ptrs->at(i) = nullptr;
    }
}

/**
 * Initializes a tuple
 *
 * @param tuplePtr
 * @param values
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::initTuple(llvm::Value *tuplePtr, std::vector<llvm::Value *> *values) {
    //fill new structure
    auto *structType = llvm::cast<llvm::StructType>(tuplePtr->getType()->getPointerElementType());
    auto types   = structType->elements();
    GazpreaTupleType *gazpreaTupleType = dynamic_cast<GazpreaTupleType *>(symbolTable->resolveTupleType(structType));
    //UserType         *userType         = dynamic_cast<UserType *>(symbolTable->resolveTupleType(structType));
    llvm::Value *element;
    llvm::Type * nType, * oType;
    std::unordered_map<int, std::pair<int, int>> *dims = nullptr;
    for(unsigned long i = 0; i < values->size(); ++i){
        llvm::Value *structElem = ir->CreateInBoundsGEP(tuplePtr, {it->getConsi32(0), it->getConsi32(i)});
        llvm::Value *ptr        = ir->CreateAlloca(types[i]->getPointerElementType());

        nType = types[i]->getPointerElementType();
        oType = values->at(i)->getType();
        element = values->at(i);

        if(gazpreaTupleType && gazpreaTupleType->getDims()){
            dims = gazpreaTupleType->getDims();
            auto p =  dims->find(i);

            if(p != dims->end()){
                int left  = (p->second.first  >= 0) ? p->second.first  : 0;
                int right = (p->second.second >= 0) ? p->second.second : 0;

                if(it->isDeclVectorType(types[i]->getPointerElementType())){
                    llvm::Type * vecElmtTy = it->getDeclScalarTypeFromVec(structElem->getType()->getPointerElementType()->getPointerElementType());
                    llvm::Value *consTy = it->getConstFromType(vecElmtTy);

                    llvm::Value * vec = et->getNewVector(consTy);
                    et->initVector(vec, it->getConsi32(left));
                    vec = it->castVectorToType(vec, vecElmtTy);
                    element = ct->typeAssCast(vec->getType()->getPointerElementType(), element, -1, it->getConsi32(left));

                    et->strictCopyVectorElements(vec, element, it->getConsi32(-1), it->getConsi32(0));

                    ir->CreateStore(ptr, structElem);
                    ir->CreateStore(vec, ptr);
                }
                else if(it->isDeclMatrixType(types[i]->getPointerElementType())){
                    llvm::Type * matElmtTy = it->getDeclScalarTypeFromMat(structElem->getType()->getPointerElementType()->getPointerElementType());
                    llvm::Value *consTy = it->getConstFromType(matElmtTy);

                    llvm::Value * mat = et->getNewMatrix(consTy);
                    et->initMatrix(mat, it->getConsi32(left), it->getConsi32(right));
                    mat = it->castMatrixToType(mat, matElmtTy);

                    llvm::Value *rows, *cols;
                    rows = it->getConsi32(left);
                    cols = it->getConsi32(right);
                    element = ct->typeAssCast(mat->getType()->getPointerElementType(), element, -1, rows, cols);

                    et->strictCopyMatrixElements(mat, element, it->getConsi32(-1), it->getConsi32(0));

                    ir->CreateStore(ptr, structElem);
                    ir->CreateStore(mat, ptr);
                }
                else if(intervalTy->getPointerTo() == types[i]->getPointerElementType()){
                    llvm::Value *srcLower = it->getValFromStruct(values->at(i), INTERVAL_MIN);
                    llvm::Value *srcUpper = it->getValFromStruct(values->at(i), INTERVAL_MAX);
                    llvm::Value *interval = et->getNewInterval(srcLower, srcUpper);
                    ir->CreateStore(ptr, structElem);
                    ir->CreateStore(interval, ptr);
                }
                continue;
            }
        }

        if((nType != oType) && (nType == realTy)&& (oType == intTy))
            element = ir->CreateSIToFP(element, realTy);

        ir->CreateStore(element, ptr);
        ir->CreateStore(ptr, structElem);
    }
    return tuplePtr ;
}