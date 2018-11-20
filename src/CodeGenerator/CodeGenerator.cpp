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
extern llvm::Type *intervalTy;

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
        if(it->isStructType(ret)){
            llvm::Value * realRet = ir->CreateAlloca(ir->getCurrentFunctionReturnType()->getPointerElementType());
            realRet = it->initTuple(realRet, it->getValueVectorFromStruct(ret));
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
    //todo - hopefully we can handle the iterator (in) in it's own node
    //                THEN WE CAN DELETE THIS CLASS
    return nullptr;
}

/**
 * Creates declarations statements and adds the
 * new variables into the symbolTable
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(DeclNode *node) {
    llvm::Value *val = visit(node->getExpr());
    llvm::Value *ptr = nullptr;

    if        (node->getTypeIds()->empty() && (node->getType() == TUPLE)) {
        ptr = val;
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    } else if (node->getTypeIds()->empty() && it->isStructType(val)) {
        ptr = ir->CreateAlloca(val->getType()->getPointerElementType());
        ptr = it->initTuple(ptr, it->getValueVectorFromStruct(val));
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), ptr);
    } else if (node->getTypeIds()->empty()) {
        ptr = ir->CreateAlloca(val->getType());
        ir->CreateStore(val, ptr);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    } else if (node->getTypeIds()->size() == 1) {
        llvm::Type *type = symbolTable->resolveType(node->getTypeIds()->at(0))->getTypeDef();
        node->setLlvmType(type);

        ptr = ir->CreateAlloca(type);
        if(val == nullptr) {
            if (!(it->setNull(type, ptr))) {
                std::cerr << "Unable to initialize to null at line " << node->getLine() << ". Aborting...\n";
            }
            symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), ptr);
            return nullptr;
        }

        if (type->isStructTy()) {
            ptr = it->initTuple(ptr, it->getValueVectorFromStruct(val));
        } else {
            val = ct->typeAssCast(type, val, node->getLine());
            ir->CreateStore(val, ptr);
        }

        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    }

    symbolTable->resolveSymbol(node->getID())->setPtr(ptr);
    //TODO - ALL OTHER CASES
    return nullptr;
}

/**
 * Deals with assignments
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(AssignNode *node) {
    Symbol *left, *right;
    left = symbolTable->resolveSymbol(node->getID());
    assert(!left->isConstant());

    if (dynamic_cast<IDNode *>(node->getExpr())) {
        auto idNode = (IDNode *) node->getExpr();
        right = symbolTable->resolveSymbol(idNode->getID());
        std::string outString = "std_output()";
        std::string inString = "std_input()";

        if (((left->getType() == INSTREAM) || (left->getType() == OUTSTREAM)) &&
            left->getType() != right->getType()) {

            auto *error = new ScalarNode(outString, inString, node->getLine());
            eb ->printError(error);
        } else if (((left->getType() == INSTREAM) || (left->getType() == OUTSTREAM)) &&
            left->getType() == right->getType()) {
            return nullptr;
        }
    }

    llvm::Value *val = visit(node->getExpr());
    llvm::Value *ptr = left->getPtr();
    if (val) {
        if (it->isStructType(left->getPtr())) {
            ptr = it->initTuple(ptr, it->getValueVectorFromStruct(val));
            left->setPtr(ptr);
        } else {
            val = ct->typeAssCast(ptr->getType()->getPointerElementType(), val, node->getLine());
            ir->CreateStore(val, ptr);
        }

    } else if (!(it->setNull(ptr->getType()->getPointerElementType(), ptr))){
            std::cerr << "Unable to initialize to null at line" << node->getLine() <<". Aborting...\n";
    }
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
    if (ptr->getType()->getPointerElementType()->isStructTy())
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

    et->aliScanf(ptr);
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
 *
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(StreamDeclNode *node) {
    symbolTable->addSymbol(node->getId(), node->getType(), false);
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
        type = parseStructType(dynamic_cast<TupleType *>(node->getTuple()));
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
 * Deals with implicit and explicit casting.
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(CastExprNode *node) {
    llvm::Type *type;
    llvm::Value *expr;
    std::string temp = node->getTypeString();

    if(node->getTuple() || it->isStructType(visit(node->getExpr()))) {
        // Checks if casting a tuple to another type
        if(!node->getTuple()) {
            std::string left = node->getTypeString();
            std::string right = "tuple(*)";
            auto *error = new ScalarNode(left, right, node->getLine()); // print error message and abort
            eb->printError(error);
        }

        auto *values = new std::vector<llvm::Value *>();
        llvm::StructType *types = parseStructType(dynamic_cast<TupleType *>(node->getTuple()));
        llvm::Value *exprP = visit(node->getExpr());
        llvm::Value *ptr = ir->CreateAlloca(types);


        // Checks if casting another type to a tuple
        if(!it->isStructType(exprP)) {
            std::string left = "tuple(*)";
            std::string right = it->getType(exprP->getType(), exprP);
            auto *error = new ScalarNode(left, right, node->getLine()); // print error message and abort
            eb->printError(error);
        }

        // Checks if tuples are different size
        if(types->getStructNumElements() != exprP->getType()->getPointerElementType()->getStructNumElements()){
            std::cerr << "Mismatching tuple lengths on line " << node->getLine() <<". Aborting...\n";
            exit(1);
        }

        for(unsigned long i = 0; i < types->elements().size(); ++i) {
            expr = it->getValFromTuple(exprP, it->getConsi32(i));
            type = types->elements()[i];

            expr = ct->varCast(type->getPointerElementType(), expr, node->getLine());
            values->push_back(expr);
        }
        return it->initTuple(ptr, values);

    } else {
        expr = visit(node->getExpr());
        GazpreaType *gazpreaType = symbolTable->resolveType(node->getTypeString());
        type = gazpreaType->getTypeDef();

        return ct->varCast(type, expr, node->getLine());
    }
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
 * Deals with global constants
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(GlobalDeclNode *node) {
    llvm::Value *val = visit(node->getExpr());

    //set constant
    auto *cons = llvm::cast<llvm::Constant>(val);//llvm::ConstantInt::get(intTy, 0, true);
    auto *consLoc =
            llvm::cast<llvm::GlobalVariable>(
                    mod->getOrInsertGlobal(node->getID(), cons->getType())
            );
    consLoc->setInitializer(cons);

    //send signal eleven if anyone tries to assign to it
    symbolTable->addSymbol(node->getID(), UNDEF, true, nullptr);
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

llvm::Value *CodeGenerator::visit(IndexNode *node) {
    return nullptr;
}

llvm::Value *CodeGenerator::visit(IntervalNode *node) {
    return nullptr;
}