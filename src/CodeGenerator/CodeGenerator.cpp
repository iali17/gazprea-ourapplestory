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
    et->registerPow();

    symbolTable->pushNewScope("_globalScope_");

    //add base types
    symbolTable->addBaseType("boolean"  , boolTy);
    symbolTable->addBaseType("character", charTy);
    symbolTable->addBaseType("integer"  , intTy);
    symbolTable->addBaseType("real"     , realTy);
    symbolTable->addBaseType("void"     , llvm::Type::getVoidTy(*globalCtx));

    symbolTable->addSymbol("std_input()" , INSTREAM,  false);
    symbolTable->addSymbol("std_output()", OUTSTREAM, false);

    unsigned long i = 0;
    for(i = 0; i < node->nodes->size(); i++){
        ASTBaseVisitor::visit(node->nodes->at(i)) ;
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

llvm::Value *CodeGenerator::visit(NullNode *node) {
    return nullptr;
}

llvm::Value *CodeGenerator::visit(CondNode *node) {
    auto condBuilder = new CondBuilder(globalCtx, ir, mod);

    unsigned long i = 0;
    for(i = 0; i < node->getConds()->size(); i++){
        if(i){
            llvm::Value *cond = visit(node->getConds()->at(i));
            condBuilder->beginElseIf(ct->varCast(boolTy, cond, 0));
        }
        else {
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

llvm::Value *CodeGenerator::visit(LoopNode *node) {
    auto whileBuilder = new WhileBuilder(globalCtx, ir, mod);
    whileStack->push(whileBuilder);
    whileBuilder->beginWhile();
    if(node->getControl()) {
        llvm::Value *cond = visit(node->getControl());
        whileBuilder->insertControl(ct->varCast(boolTy, cond, node->getLine()));
    }
    else
        whileBuilder->insertControl(it->geti1(1));

    visit(node->getBlock());
    whileBuilder->endWhile();
    whileStack->pop();
    return nullptr;
}

/**
 * TODO
 * @param node
 * @return
 */
llvm::Value *CodeGenerator::visit(DoLoopNode *node) {
    auto whileBuilder = new WhileBuilder(globalCtx, ir, mod);
    whileStack->push(whileBuilder);
    whileBuilder->beginWhile();
    visit(node->getBlock());
    whileBuilder->beginInsertControl();
    llvm::Value *cond = visit(node->getControl());
    whileBuilder->insertControl(ct->varCast(boolTy, cond, node->getLine()));
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
    llvm::Value *val = visit(node->getExpr());
    llvm::Value *ptr = nullptr;

    if      (node->getTypeIds()->empty() && (node->getType() == TUPLE)){
        ptr = val;
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    }
    else if (node->getTypeIds()->empty() && it->isStructType(val)) {
        ptr = ir->CreateAlloca(val->getType()->getPointerElementType());
        ptr = it->initTuple(ptr, it->getValueVectorFromStruct(val));
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), ptr);
    }
    else if (node->getTypeIds()->empty()) {
        ptr = ir->CreateAlloca(val->getType());
        ir->CreateStore(val, ptr);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    }
    else if (node->getTypeIds()->size() == 1){
        llvm::Type *type = symbolTable->resolveType(node->getTypeIds()->at(0))->getTypeDef();

        node->setLlvmType(type);

        ptr = ir->CreateAlloca(type);
        if(val == nullptr) {
            if (!(it->setNull(type, ptr))){
                std::cerr << "Unable to initialize to null\n";
            }
            symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), ptr);
            return nullptr;
        }

        if (type->isStructTy()) {
            ptr = it->initTuple(ptr, it->getValueVectorFromStruct(val));
        } else {
            val = ct->typeAssCast(type, val, -1);
            ir->CreateStore(val, ptr);
        }

        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant());
    }

    symbolTable->resolveSymbol(node->getID())->setPtr(ptr);
    //TODO - ALL OTHER CASES
    return nullptr;
}

llvm::Value *CodeGenerator::visit(AssignNode *node) {
    Symbol *left, *right;
    left = symbolTable->resolveSymbol(node->getID());
    assert(!left->isConstant());

    if(dynamic_cast<IDNode *>(node->getExpr())){
        auto idNode = (IDNode *) node->getExpr();
        right = symbolTable->resolveSymbol(idNode->getID());
        if (((left->getType() == INSTREAM) || (left->getType() == OUTSTREAM)) &&
            left->getType() != right->getType()){
            std::cerr << "Incompatable stream assignment\n";
            return nullptr;
        }
        else if (((left->getType() == INSTREAM) || (left->getType() == OUTSTREAM)) &&
            left->getType() == right->getType()){
            return nullptr;
        }
    }

    llvm::Value *val = visit(node->getExpr());
    llvm::Value *ptr = left->getPtr();
    if(val) {
        if (it->isStructType(left->getPtr())) {
            ptr = it->initTuple(ptr, it->getValueVectorFromStruct(val));
            left->setPtr(ptr);
        } else {
            val = ct->typeAssCast(ptr->getType()->getPointerElementType(), val, node->getLine());
            ir->CreateStore(val, ptr);
        }

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

    Symbol *symbol   = symbolTable->resolveSymbol(node->getID());
    llvm::Value *ptr = symbol->getPtr();
    if (ptr->getType()->getPointerElementType()->isStructTy())
        return ptr;
    return ir->CreateLoad(ptr);
}

// Todo: Fix identity
llvm::Value *CodeGenerator::visit(IdnNode *node) {
    llvm::Value *newNode = it->getConsi32(1);
    newNode->setName("IdnNode");

    return newNode;
}

llvm::Value *CodeGenerator::visit(InputNode *node) {
    Symbol *symbol;
    llvm::Value *ptr;
    if(symbolTable->resolveSymbol(node->getStreamName())->getType() != INSTREAM){
        std::cerr << "Not proper stream\nAborting...\n";
        exit(1);
    }

    symbol = symbolTable->resolveSymbol(node->getStoreID());
    ptr    = symbol->getPtr();

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
        exit(1);
    }

    symbolTable->addUserType(node->getId(), type);

    return nullptr;
}

llvm::Value *CodeGenerator::visit(CastExprNode *node) {
    llvm::Type *type;
    llvm::Value *expr;
    std::string temp = node->getTypeString();

    if(node->getTuple()) {
        std::vector<llvm::Value *> *values = new std::vector<llvm::Value *>();
        llvm::StructType *types = parseStructType(dynamic_cast<TupleType *>(node->getTuple()));
        llvm::Value *exprP = visit(node->getExpr());
        llvm::Value *ptr = ir->CreateAlloca(types);

        //assert(types->getStructNumElements() == exprVector->size());

        for(int i = 0; i < types->elements().size(); i++) {
            expr = it->getValFromTuple(exprP, it->getConsi32(i));
            type = types->elements()[i];

            expr = ct->varCast(type->getPointerElementType(), expr, node->getLine());
            values->push_back(expr);
        }

        return it->initTuple(ptr, values);
    }
    else {
        expr = visit(node->getExpr());
        GazpreaType *gazpreaType = symbolTable->resolveType(node->getTypeString());
        type = gazpreaType->getTypeDef();

        return ct->varCast(type, expr, node->getLine());
    }
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

llvm::Value *CodeGenerator::visit(TupleNode *node) {
    auto *values = new std::vector<llvm::Value *>();
    auto *types  = new std::vector<llvm::Type  *>();

    //build vector for values and types
    for(unsigned long i = 0; i < node->getElements()->size(); i++){
        llvm::Value * element = visit(node->getElements()->at(i));
        values->push_back(element);
        types->push_back(element->getType()->getPointerTo());
    }

    //build structtype and allocate
    llvm::StructType *tuple;
    tuple = tuple->create(*types, "tuple");
    llvm::Value *tuplePtr = ir->CreateAlloca(tuple);

    //fill new structure
    for(unsigned long i = 0; i < node->getElements()->size(); i++){
        llvm::Value *structElem = ir->CreateInBoundsGEP(tuplePtr, {it->getConsi32(0), it->getConsi32(i)});
        llvm::Value *ptr        = ir->CreateAlloca(values->at(i)->getType());
        ir->CreateStore(values->at(i), ptr);
        ir->CreateStore(ptr, structElem);
    }

    symbolTable->addTupleType(tuple, new std::unordered_map<std::string, int>, types);
    return tuplePtr ;
}

llvm::Value *CodeGenerator::visit(TupleNode *node, llvm::StructType *tuple) {
    auto *values = new std::vector<llvm::Value *>();
    auto types   = tuple->elements();

    for(unsigned long i = 0; i < types.size(); i++){
        llvm::Value * element;
        if (not(node)){
            element = it->getNull(types[i]);
        }
        else
            element = visit(node->getElements()->at(i));

        llvm::Type *memberType  = types[i]->getPointerElementType();
        element = ct->typeAssCast(memberType, element, -1);
        values->push_back(element);
    }

    llvm::Value *tuplePtr = ir->CreateAlloca(tuple);
    //fill new structure
    for(unsigned long i = 0; i < node->getElements()->size(); i++){
        llvm::Value *structElem = ir->CreateInBoundsGEP(tuplePtr, {it->getConsi32(0), it->getConsi32(i)});
        llvm::Value *ptr        = ir->CreateAlloca(values->at(i)->getType());
        ir->CreateStore(values->at(i), ptr);
        ir->CreateStore(ptr, structElem);
    }
    return tuplePtr ;
}

// todo check if left hand side tuple type fits the right hand side tuple expr
llvm::Value *CodeGenerator::visit(TupleDeclNode *node) {
    llvm::StructType * structType = parseStructType(dynamic_cast<TupleType *>(node->getTupleTypes()));
    TupleNode *tupleNode = nullptr;
    llvm::Value *ptr;

    //get the struct type from the definition
    structType = parseStructType(dynamic_cast<TupleType *>(node->getTupleTypes()));

    tupleNode = dynamic_cast<TupleNode *>(node->getExpr());

    //cover null declaration cases
    if(not(tupleNode) && dynamic_cast<NullNode *>(node->getExpr())){
        ptr = initTuple(NULLTY, structType);
    }
    else if (not(tupleNode) && dynamic_cast<IdnNode *>(node->getExpr())) {
        ptr = initTuple(IDENTITY, structType);
    }
    else if (not(tupleNode) && (dynamic_cast<IDNode *>(node->getExpr()) || dynamic_cast<CastExprNode *>(node->getExpr()))){
        llvm::Value * tuplePtr = visit(node->getExpr());
        ptr = ir->CreateAlloca(structType);
        ptr = it->initTuple(ptr, it->getValueVectorFromStruct(tuplePtr));
    }
    else {
        ptr = visit(tupleNode, structType);
    }

    symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), ptr);
    return nullptr;
}

llvm::Value *CodeGenerator::initTuple(int INIT, llvm::StructType *tuple) {
    auto *values = new std::vector<llvm::Value *>();
    auto types   = tuple->elements();
    llvm::Value * element;

    for(unsigned long i = 0; i < types.size(); i++){
        if (INIT == NULLTY){
            element = it->getNull(types[i]->getPointerElementType());
        }
        else if (INIT == IDENTITY){
            element = it->getIdn(types[i]->getPointerElementType());
        }
        values->push_back(element);
    }

    llvm::Value *tuplePtr = ir->CreateAlloca(tuple);
    return it->initTuple(tuplePtr, values);
}

llvm::Value *CodeGenerator::visit(TupleType *node) {
    auto * declNodes = node->getDecls();
    auto members = new std::vector<llvm::Type *>;
    auto *memberNames = new std::unordered_map<std::string, int>;
    int i = 0;
    for (auto element : * declNodes) {
        members->push_back(element->getLlvmType()->getPointerTo());
        if(!(((DeclNode *) element)->getID().empty()))
            memberNames->insert(std::pair<std::string, int> (((DeclNode *) element)->getID(), i));
        i++;
    }

    auto * newStruct = llvm::StructType::create(*members);
    symbolTable->addTupleType(newStruct, memberNames, members); //this is where we add the struct to the symbol table
    return ir->CreateAlloca(newStruct);
}

llvm::Value *CodeGenerator::visit(PythonTupleAssNode *node) {
    Symbol *var;
    llvm::Value *expr;
    std::vector<std::string> variables = node->getIDs();
    llvm::Value *exprP = visit(node->getExpr());

    for(int i = 0; i < variables.size(); i++) {
        var = symbolTable->resolveSymbol(variables.at(i));

        assert(!var->isConstant());

        expr = it->getValFromTuple(exprP, it->getConsi32(i));
        expr = ct->typeAssCast(var->getPtr()->getType()->getPointerElementType(), expr, node->getLine());

        ir->CreateStore(expr, var->getPtr());
    }

    return nullptr;
}

llvm::Value *CodeGenerator::visit(GlobalDeclNode *node) {
    llvm::Value *val = visit(node->getExpr());
    llvm::Type  *type;

    if      (node->getTypeIds()->empty()){
        type = val->getType();
    }
    else if (node->getTypeIds()->size() == 1){
        type = symbolTable->resolveType(node->getTypeIds()->at(0))->getTypeDef();
    }
    else {
        //todo - make the constant
    }

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

llvm::Value *CodeGenerator::visit(GlobalRefNode *node) {
    llvm::GlobalVariable *global = mod->getGlobalVariable(node->getGlobalName());
    return global->getInitializer();
}

/**
 * THIS RETURNS THE VALUE NOT THE POINTER
 * @param node
 * @return
 */
llvm::Value *CodeGenerator::visit(IndexTupleNode *node) {
    Symbol *symbol   = symbolTable->resolveSymbol(node->getIdNode()->getID());
    llvm::Value *ptr = symbol->getPtr();
    llvm::Value *idx = getIndexForTuple(node->getIndex(), ptr);

    return it->getValFromTuple(ptr, idx);
}

llvm::Value *CodeGenerator::visit(TupleMemberAssNode *node) {
    std::string symbolName = node->getLHS()->getIdNode()->getID();
    Symbol *symbol         = symbolTable->resolveSymbol(symbolName);
    llvm::Value *ptr       = symbol->getPtr();
    llvm::Value *idx       = getIndexForTuple(node->getLHS()->getIndex(), ptr);
    llvm::Value *val       = visit(node->getExpr());

    ir->CreateStore(val, it->getPtrFromTuple(ptr,idx));
    return nullptr;
}

llvm::Value *CodeGenerator::visit(TupleInputNode *node) {
    Symbol *symbol;
    llvm::Value *ptr, *idx;
    if(symbolTable->resolveSymbol(node->getStreamName())->getType() != INSTREAM){
        std::cerr << "Not proper stream\nAborting...\n";
        exit(1);
    }

    //resolve symbol
    IndexTupleNode *tupleInputNode = node->getIndexTupleNode();
    std::string tupleID            = tupleInputNode->getIdNode()->getID();
    symbol                         = symbolTable->resolveSymbol(tupleID);

    //get index then get pointer to element
    ptr = symbol->getPtr();
    idx = getIndexForTuple(tupleInputNode->getIndex(), ptr);
    ptr = it->getPtrFromTuple(ptr, idx);

    et->aliScanf(ptr);
    return nullptr;
}
