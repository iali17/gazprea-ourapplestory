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
extern llvm::Type *strTy;

/**
 * Pushes global scope and inserts base types into symbolTable.
 * Also initializes external functions.
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(FileNode *node) {
    // register external functions
    et->registerPrintf();
    et->registerFree();
    et->registerCalloc();
    et->registerScanf();
    et->registerPow();
    et->registerVectorFunctions();
    et->registerIntervalFunctions();
    et->registerMatrixFunctions();

    symbolTable->pushNewScope("_globalScope_");

    //add base types
    symbolTable->addBaseType("boolean"  , boolTy);
    symbolTable->addBaseType("character", charTy);
    symbolTable->addBaseType("integer"  , intTy);
    symbolTable->addBaseType("real"     , realTy);
    symbolTable->addBaseType("void"     , llvm::Type::getVoidTy(*globalCtx));
    symbolTable->addBaseType("vector"   , vecTy);
    symbolTable->addBaseType("matrix"   , matrixTy);
    symbolTable->addBaseType("interval" , intervalTy);
    symbolTable->addBaseType("string"   , strTy);


    symbolTable->addSymbol("std_input()" , INSTREAM,  false);
    symbolTable->addSymbol("std_output()", OUTSTREAM, false);

    for (unsigned long i = 0; i < node->nodes->size(); ++i){
        ASTBaseVisitor::visit(node->nodes->at(i)) ;
    }

    symbolTable->popScope();
    return nullptr;
}

/**
 * Pushes a new scope when entering a basic block,
 * and pops scope when leaving block.
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(BasicBlockNode *node) {
    symbolTable->pushNewScope();
    for (unsigned long i = 0; i < node->nodes->size(); ++i) {
        ASTBaseVisitor::visit(node->nodes->at(i));
    }
    symbolTable->popScope();
    return nullptr;
}

/**
 * Goes to super class
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(ASTNode *node) {
    return ASTBaseVisitor::visit(node);
}

/**
 * Initializes output file where llvm code is written to
 *
 * @param outFile
 */
CodeGenerator::CodeGenerator(char *outFile) : outFile(outFile) {
}

//Todo:
/**
 *
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(ReturnNode *node) {
    if (node->getExpr() == nullptr) {
        ir->CreateRetVoid();
    } else {
        llvm::Value *ret = visit(node->getExpr());
        if(it->isTupleType(ret)){
            llvm::Value * realRet = ir->CreateAlloca(ir->getCurrentFunctionReturnType()->getPointerElementType());
            realRet = it->initTuple(realRet, it->getValueVectorFromTuple(ret));
            ir->CreateRet(realRet);
        } else {
            ir->CreateRet(ret);
        }

    }
    return nullptr;
}

/**
 * Returns a llvm::Value *ptr to an integer type
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(INTNode *node) {
    return it->getConsi32(node->value);
}

/**
 * Returns a llvm::Value *ptr to a real type
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(RealNode *node) {
    float val = node->getVal();
    return it->getReal(val);
}

/**
 * Returns a llvm::Value *ptr to a char type
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(CharNode *node) {
    char val = node->getVal();
    return it->geti8(val);
}

/**
 * Returns a llvm::Value *ptr to a bool type
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(BoolNode *node) {
    bool val = node->getVal();
    return it->geti1(val);
}

/**
 * Doesn't do much
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(NullNode *node) {
    return nullptr;
}

/**
 * Creates if, else, and else if statments,
 * also casts expressons to boolean type to be used
 * inside the conditions.
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(CondNode *node) {
    auto condBuilder = new CondBuilder(globalCtx, ir, mod);

    unsigned long i = 0;
    for(i = 0; i < node->getConds()->size(); ++i){
        if(i){
            llvm::Value *cond = visit(node->getConds()->at(i));
            condBuilder->beginElseIf(ct->varCast(boolTy, cond, 0));
        } else {
            llvm::Value *cond = visit(node->getConds()->at(i));
            condBuilder->beginIf(ct->varCast(boolTy, cond, 0));
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

/**
 * Creates simple while loops
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(LoopNode *node) {
    auto whileBuilder = new WhileBuilder(globalCtx, ir, mod);
    whileStack->push(whileBuilder);
    //begin while
    whileBuilder->beginWhile();
    //insert control
    if (node->getControl()) {
        llvm::Value *cond = visit(node->getControl());
        whileBuilder->insertControl(ct->varCast(boolTy, cond, node->getLine()));
    } else {
        whileBuilder->insertControl(it->geti1(1));
    }

    //generate body
    visit(node->getBlock());
    //end while
    whileBuilder->endWhile();
    whileStack->pop();
    return nullptr;
}

/**
 * Creates do while loop statements
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(DoLoopNode *node) {
    auto whileBuilder = new WhileBuilder(globalCtx, ir, mod);
    whileStack->push(whileBuilder);

    //begin while
    whileBuilder->beginWhile();
    //insert body
    visit(node->getBlock());
    //insert control
    whileBuilder->beginInsertControl();
    llvm::Value *cond = visit(node->getControl());
    whileBuilder->insertControl(ct->varCast(boolTy, cond, node->getLine()));\
    //end while
    whileBuilder->endWhile();
    whileStack->pop();
    return nullptr;
}

llvm::Value *CodeGenerator::visit(InLoopNode *node) {
    llvm::Value * domain = visit(node->getDomain());

    //loop idx
    llvm::Value * curIdx = it->getConsi32(0);
    llvm::Value * curIdxPtr = ir->CreateAlloca(intTy);
    ir->CreateStore(curIdx, curIdxPtr);

    //domain variables
    llvm::Value *domainValPtrs  = it->getPtrFromStruct(domain, VEC_ELEM_INDEX);
    llvm::Type  *domainType     = it->getVectorElementType(domain);
    llvm::Value *curDomainVal;
    llvm::Value *curDomainValPtr;
    llvm::Value *numDomainElmts = it->getValFromStruct(domain, VEC_LEN_INDEX);

    //add loop var to symbol table
    symbolTable->pushNewScope();
    llvm::Value *curLoopVarPtr = ir->CreateAlloca(domainType);
    symbolTable->addSymbol(node->getLoopVar(), UNDEF, true, curLoopVarPtr);

    auto * wb = new WhileBuilder(globalCtx, ir, mod);
    whileStack->push(wb);
    wb->beginWhile();

    //load idx
    curIdx = ir->CreateLoad(curIdxPtr);

    //update variable
    curDomainValPtr = ir->CreateGEP(domainValPtrs, curIdx);
    curDomainVal    = ir->CreateLoad(curDomainValPtr);
    ir->CreateStore(curDomainVal, curLoopVarPtr);

    wb->beginInsertControl();
    wb->insertControl(ir->CreateICmpSLT(curIdx, numDomainElmts));

    //load idx again
    curIdx = ir->CreateLoad(curIdxPtr);

    //visit the block
    ASTNode * b = node->getBlock();
    visit(b);

    //increment idx
    curIdx = ir->CreateAdd(curIdx, it->getConsi32(1));
    ir->CreateStore(curIdx, curIdxPtr);

    wb->endWhile();
    whileStack->pop();
    symbolTable->popScope();

    return nullptr;
}

/**
 * Returns a ptr to the value of the id.
 *
 * @param node
 * @return llvm::Value *ptr to type if exists,
 *         otherwise return nullptr if value is null
 */
llvm::Value *CodeGenerator::visit(IDNode *node) {
    if(node->getID() == "null"){
        return nullptr;
    }

    Symbol *symbol   = symbolTable->resolveSymbol(node->getID());
    llvm::Value *ptr = symbol->getPtr();
    if (it->isStructType(ptr))
        return ptr;
    return ir->CreateLoad(ptr);
}

// Todo: Fix identity
/**
 * Returns a llvm::Value *ptr to an integer type with the name "IdnNode"
 * this deals with identitys.
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(IdnNode *node) {
    llvm::Value *newNode = it->getConsi32(1);
    newNode->setName("IdnNode");

    return newNode;
}

/**
 * Reads from stdin into variable
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(InputNode *node) {
    Symbol *symbol;
    llvm::Value *ptr;
    if (symbolTable->resolveSymbol(node->getStreamName())->getType() != INSTREAM) {
        std::cerr << "Not proper stream on line" << node->getLine() << ". Aborting...\n";
        exit(1);
    }

    symbol = symbolTable->resolveSymbol(node->getStoreID());
    ptr    = symbol->getPtr();
    llvm::Value * stream = symbolTable->resolveSymbol(node->getStreamName())->getPtr();

    et->aliScanf(stream,ptr,it->getConstFromType(ptr->getType()->getPointerElementType()));
    return nullptr;
}

/**
 * Writes values to output stream
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(OutputNode *node) {
    if (symbolTable->resolveSymbol(node->getStreamName())->getType() != OUTSTREAM){
        std::cerr << "Not proper stream on line " << node->getLine() << ". Aborting...\n";
        exit(1);
    }

    if (dynamic_cast<IdnNode *>(node->getExpr())) {
        et->print(it->geti8(1));
    } else if (dynamic_cast<NullNode *>(node->getExpr())) {
        et->print(it->geti8(0));
    } else {
        llvm::Value *expr = visit(node->getExpr());
        et->print(expr);
    }

    return nullptr;
}

/**
 * Deals with pushing a new user defined type to the scope.
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(TypeDefNode *node) {
    llvm::Type *type;

    if(node->getTuple())
        type = parseStructType(dynamic_cast<TupleTypeNode *>(node->getTuple()));
    else if(node->getCustomType() == "integer")
        type = intTy;
    else if(node->getCustomType() == "real")
        type = realTy;
    else if(node->getCustomType() == "character")
        type = charTy;
    else if(node->getCustomType() == "boolean")
        type = boolTy;
    else {
        // gotta do for matrix type
        std::cerr << "Not proper defined type on line " << node->getLine() <<". Aborting...\n";
        exit(1);
    }

    symbolTable->addUserType(node->getId(), type);
    return nullptr;
}

/**
 * Deals with continue statements
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(ContinueNode *node) {
    if(whileStack->top()){
        ir->CreateBr(whileStack->top()->getStartWhileBB());
    }
    else {
        std::cerr << "Continue on line " << node->getLine() << " does not reside in while loop\n";
    }
    return nullptr;
}

/**
 * Deals with break statements
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(BreakNode *node) {
    if(whileStack->top()){
        ir->CreateBr(whileStack->top()->getMergeBB());
    }
    else {
        std::cerr << "Break on line " << node->getLine() << " does not reside in while loop\n";
    }
    return nullptr;
}

/**
 * Returns the value of the global variable
 *
 * @param node
 * @return llvm::value *
 */
llvm::Value *CodeGenerator::visit(GlobalRefNode *node) {
    llvm::GlobalVariable *global = mod->getGlobalVariable(node->getGlobalName());
    return global->getInitializer();
}

llvm::Value *CodeGenerator::visit(StreamStateNode *node) {
    llvm::Value *ptr = symbolTable->resolveSymbol(node->getID())->getPtr();
    llvm::Value *ret = ir->CreateInBoundsGEP(ptr, {it->getConsi32(0), it->getConsi32(1)});

    return ir->CreateLoad(ret);
}
