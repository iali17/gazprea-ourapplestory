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
    bool isIdn             = dynamic_cast<IdnNode *>(node->getExpr());
    bool isNull            = dynamic_cast<NullNode *>(node->getExpr());
    llvm::Value *val       = visit(node->getExpr());
    llvm::Value *destPtr   = it->getPtrFromTuple(ptr,idx);
    llvm::Value *loaded    = ir->CreateLoad(destPtr);
    bool isVector          = it->isVectorType(loaded);
    bool isMatrix          = it->isMatrixType(loaded);
    llvm::Type *destTy     = destPtr->getType()->getPointerElementType();


    //deal with null and idn
    if(isIdn && (isMatrix || isVector)){
        setIdentityVecOrMat(loaded);
        return nullptr;
    }
    else if (isNull && (isMatrix || isVector)){
        setNullVecOrMat(loaded);
        return nullptr;
    }

    if(isVector){
        llvm::Value * len;
        if(it->isVectorType(val))
            len = it->getValFromStruct(val, VEC_LEN_INDEX);
        else
            len = it->getValFromStruct(loaded, VEC_LEN_INDEX);


        val = ct->typeAssCast(loaded->getType()->getPointerElementType(), val, node->getLine(), len);
        et->strictCopyVectorElements(loaded, val, it->getConsi32(node->getLine()), it->getConsi32(0));

        return nullptr;
    }
    else if(isMatrix){
        llvm::Value *rows;
        llvm::Value *cols;

        if(it->isMatrixType(val)){
            rows = it->getValFromStruct(val, MATRIX_NUMROW_INDEX);
            cols = it->getValFromStruct(val, MATRIX_NUMCOL_INDEX);
        }
        else{
            rows = it->getValFromStruct(loaded, MATRIX_NUMROW_INDEX);
            cols = it->getValFromStruct(loaded, MATRIX_NUMCOL_INDEX);
        }

        val = ct->typeAssCast(loaded->getType()->getPointerElementType(), val, node->getLine(), rows, cols);
        et->strictCopyVectorElements(loaded, val, it->getConsi32(node->getLine()), it->getConsi32(0));
        return nullptr;
    }
    else if(it->isIntervalType(loaded)){
        llvm::Value *left;
        llvm::Value *right;
        if(isIdn){
            left  = it->getConsi32(1);
            right = it->getConsi32(1);
        }
        else if(isNull){
            left  = it->getConsi32(0);
            right = it->getConsi32(0);
        }
        else{
            left  = it->getValFromStruct(val, INTERVAL_MIN);
            right = it->getValFromStruct(val, INTERVAL_MAX);
        }

        llvm::Value *lDest = it->getPtrFromStruct(loaded, INTERVAL_MIN);
        llvm::Value *rDest = it->getPtrFromStruct(loaded, INTERVAL_MAX);

        ir->CreateStore(left,  lDest);
        ir->CreateStore(right, rDest);
        return nullptr;
    }
    else if(isNull){
        it->setNull(destTy, destPtr);
    }
    else if(isIdn){
        it->setIdentity(destTy, destPtr);
    }
    else{
        val = ct->typeAssCast(destTy, val, node->getLine());
        ir->CreateStore(val, destPtr);
    }
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
    /*ptr =*/ it->getPtrFromTuple(ptr, idx);

    //et->aliScanf(ptr);
    return nullptr;
}

/**
 * Gets the value at the position of the tuple
 *
 * @param INIT
 * @param tuple
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::initTuple(int INIT, llvm::StructType *tuple) {
    auto *values = new std::vector<llvm::Value *>();
    auto types   = tuple->elements();
    auto *gazpreaTupleType = dynamic_cast<GazpreaTupleType *>(symbolTable->resolveTupleType(tuple));
    std::unordered_map<int, std::pair<int, int>> *dims = nullptr;
    llvm::Value *element = nullptr;
    int i = 0;

    for (auto type : types) {
        if(gazpreaTupleType && gazpreaTupleType->getDims()){
            dims = gazpreaTupleType->getDims();
            auto p =  dims->find(i);

            if(p != dims->end()){
                int left  = (p->second.first  >= 0) ? p->second.first  : 0;
                int right = (p->second.second >= 0) ? p->second.second : 0;

                if(it->isDeclVectorType(types[i]->getPointerElementType())){
                    llvm::Type * vecElmtTy = it->getDeclScalarTypeFromVec(type->getPointerElementType());
                    llvm::Value *consTy = it->getConstFromType(vecElmtTy);

                    llvm::Value * vec = et->getNewVector(consTy);
                    et->initVector(vec, it->getConsi32(left));
                    vec = it->castVectorToType(vec, vecElmtTy);

                    if (INIT == IDENTITY)
                        setIdentityVecOrMat(vec);

                    values->push_back(vec);

                }
                else if(it->isDeclMatrixType(types[i]->getPointerElementType())){
                    llvm::Type * matElmtTy = it->getDeclScalarTypeFromMat(type->getPointerElementType());
                    llvm::Value *consTy = it->getConstFromType(matElmtTy);

                    llvm::Value * mat = et->getNewMatrix(consTy);
                    et->initMatrix(mat, it->getConsi32(left), it->getConsi32(right));
                    mat = it->castMatrixToType(mat, matElmtTy);

                    if (INIT == IDENTITY)
                        setIdentityVecOrMat(mat);

                    values->push_back(mat);
                }
                else if(intervalTy->getPointerTo() == types[i]->getPointerElementType()){
                    llvm::Value *interval;
                    if (INIT == IDENTITY)
                        interval = et->getNewInterval(it->getConsi32(1), it->getConsi32(1));
                    else
                        interval = et->getNewInterval(it->getConsi32(0), it->getConsi32(0));
                    values->push_back(interval);
                }
                ++i;
                continue;
            }
        }
        if (INIT == NULLTY){
            element = it->getNull(type->getPointerElementType());
        }
        else if (INIT == IDENTITY){
            element = it->getIdn(type->getPointerElementType());
        }
        values->push_back(element);
        ++i;
    }

    llvm::Value *tuplePtr = ir->CreateAlloca(tuple);
    return initTuple(tuplePtr, values);
}

/**
 * Creates struct definition and allocates space
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(TupleTypeNode *node) {
    llvm::StructType * newStruct = parseStructType(node);
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

    for(unsigned long i = 0; i < variables.size(); ++i) {
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
    for(unsigned long i = 0; i < node->getElements()->size(); ++i){
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
    //auto * dims = new std::unordered_map<int, std::pair<int, int>>;
    auto *gazTy = symbolTable->resolveTupleType(tuple);

    for(unsigned long i = 0; i < types.size(); ++i){
        llvm::Value * element;
        if (not(node)){
            element = it->getNull(types[i]);
        }
        else
            element = visit(node->getElements()->at(i));

        llvm::Value * len1 = nullptr;
        llvm::Value * len2 = nullptr;

        llvm::Type *memberType  = types[i]->getPointerElementType();
        if(memberType->isPointerTy())
            memberType = memberType->getPointerElementType();

        auto dims = gazTy->getDims()->find(i);
        if(dims != gazTy->getDims()->end() && dims->second.first >= 0)
            len1 = it->getConsi32(dims->second.first);

        if(dims != gazTy->getDims()->end() && dims->second.second >= 0)
            len2 = it->getConsi32(dims->second.second);

        element = ct->typeAssCast(memberType, element, node->getLine(), len1, len2);
        values->push_back(element);
    }

    llvm::Value *tuplePtr = ir->CreateAlloca(tuple);
    //llvm::Value * element;
    //fill new structure
    for(unsigned long i = 0; i < node->getElements()->size(); ++i){
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
    structType = parseStructType(dynamic_cast<TupleTypeNode *>(node->getTupleTypes()));

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
        ptr = initTuple(ptr, it->getValueVectorFromTuple(tuplePtr));
    }
    else {
        ptr = visit(tupleNode, structType);
    }

    symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), ptr);
    return nullptr;
}
