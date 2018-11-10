//
// Created by kyle on 09/11/18.
//

#include <CodeGenerator/CodeGenerator.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;

/**
 * THIS RETURNS THE VALUE NOT THE POINTER
 *
 * @param node
 * @return llvm::Value *ptr
 */
llvm::Value *CodeGenerator::visit(IndexTupleNode *node) {
    Symbol *symbol   = symbolTable->resolveSymbol(node->getIdNode()->getID());
    llvm::Value *ptr = symbol->getPtr();
    llvm::Value *idx = getIndexForTuple(node->getIndex(), ptr);

    return it->getValFromTuple(ptr, idx);
}

/**
 * Deals with assignments to the members
 * of a tuple.
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(TupleMemberAssNode *node) {
    std::string symbolName = node->getLHS()->getIdNode()->getID();
    Symbol *symbol         = symbolTable->resolveSymbol(symbolName);
    llvm::Value *ptr       = symbol->getPtr();
    llvm::Value *idx       = getIndexForTuple(node->getLHS()->getIndex(), ptr);
    llvm::Value *val       = visit(node->getExpr());

    ir->CreateStore(val, it->getPtrFromTuple(ptr,idx));
    return nullptr;
}

/**
 * Assigns tuple member value from the input stream
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(TupleInputNode *node) {
    Symbol *symbol;
    llvm::Value *ptr, *idx;
    if(symbolTable->resolveSymbol(node->getStreamName())->getType() != INSTREAM){
        std::cerr << "Not proper stream on line " << node->getLine() <<". Aborting...\n";
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

/**
 * Gets the value at the posititon of the tuple
 *
 * @param INIT
 * @param tuple
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::initTuple(int INIT, llvm::StructType *tuple) {
    auto *values = new std::vector<llvm::Value *>();
    auto types   = tuple->elements();
    llvm::Value *element = nullptr;

    for (auto type : types) {
        if (INIT == NULLTY){
            element = it->getNull(type->getPointerElementType());
        }
        else if (INIT == IDENTITY){
            element = it->getIdn(type->getPointerElementType());
        }
        values->push_back(element);
    }

    llvm::Value *tuplePtr = ir->CreateAlloca(tuple);
    return it->initTuple(tuplePtr, values);
}

/**
 * Creates struct definition and allocates space
 *
 * @param node
 * @return llvm::Value *
 */
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

/**
 * Handles python style assignment of tuple members to variables.
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(PythonTupleAssNode *node) {
    Symbol *var;
    llvm::Value *expr;
    std::vector<std::string> variables = node->getIDs();
    llvm::Value *exprP = visit(node->getExpr());

    for(unsigned int i = 0; i < variables.size(); i++) {
        var = symbolTable->resolveSymbol(variables.at(i));

        assert(!var->isConstant());

        expr = it->getValFromTuple(exprP, it->getConsi32(i));
        expr = ct->typeAssCast(var->getPtr()->getType()->getPointerElementType(), expr, node->getLine());

        ir->CreateStore(expr, var->getPtr());
    }

    return nullptr;
}

/**
 * Creates the tuple when using var
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(TupleNode *node) {
    auto *values = new std::vector<llvm::Value *>();
    auto *types  = new std::vector<llvm::Type  *>();

    //build vector for values and types
    for (auto &i : *node->getElements()) {
        llvm::Value * element = visit(i);
        values->push_back(element);
        types->push_back(element->getType()->getPointerTo());
    }

    //build structtype and allocate
    llvm::StructType *tuple;
    tuple = llvm::StructType::create(*types, "tuple");
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

/**
 * Creates tuple from the defined tuple type
 *
 * @param node
 * @param tuple
 * @return llvm::Value *
 */
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
        element = ct->typeAssCast(memberType, element, node->getLine());
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

/**
 * Deals with tuple declarations, and pushes
 * it into the symbolTable.
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(TupleDeclNode *node) {
    llvm::StructType * structType;
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
