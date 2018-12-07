//
// Created by kyle on 23/11/18.
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
 * Generates a vector
 *
 * @param loopVar
 * @param range
 * @param exprNode
 * @return
 */
llvm::Value *CodeGenerator::generateVector(std::string loopVar, llvm::Value *range, ASTNode *exprNode) {
//variables for the return
    llvm::Value *retVec          = nullptr;
    llvm::Value *retVecIdx       = nullptr;
    llvm::Value *retVecIdxPtr    = nullptr;
    llvm::Value *retElmtsPtr     = nullptr;
    llvm::Value *retElmtPtr      = nullptr;
    llvm::Value *retElmtTypeCons = nullptr;
    llvm::Type  *retElmtType     = nullptr;

    //variables to control the loop
    llvm::Value *rangePtr    = nullptr;
    llvm::Value *curRangePtr = nullptr;
    llvm::Value *rangeVecPtr = nullptr;
    llvm::Value *rangeSize   = nullptr;

    //variables for the current loop iteration
    llvm::Value *curIdx    = nullptr;
    llvm::Value *curIdxPtr = nullptr;
    llvm::Value *curVal    = nullptr;
    llvm::Value *curValPtr = nullptr;
    llvm::Value *cond      = nullptr;

    //loop var
    llvm::Value *loopVarPtr = nullptr;
    llvm::Value *curLoopVar = nullptr;

    //visit the range, do the implicit by if needed, point to the integer elements
    rangeVecPtr = range;
    rangePtr    = it->getPtrFromStruct(rangeVecPtr, it->getConsi32(VEC_ELEM_INDEX));
    rangeSize   = it->getValFromStruct(rangeVecPtr, it->getConsi32(VEC_LEN_INDEX));

    //allocate space for the indices
    curIdxPtr    = ir->CreateAlloca(intTy);
    retVecIdxPtr = ir->CreateAlloca(intTy);
    curValPtr    = ir->CreatePointerCast(ir->CreateAlloca(intTy), charTy->getPointerTo());

    //init the loop var to have proper type
    loopVarPtr = ir->CreateAlloca(rangePtr->getType()->getPointerElementType());

    //init the indices
    curIdx    = it->getConsi32(0);
    retVecIdx = it->getConsi32(0);
    ir->CreateStore(curIdx, curIdxPtr);
    ir->CreateStore(retVecIdx, retVecIdxPtr);

    //init the return WE TREAT THIS LIKE A VECTOR WITH UNKNOWN TYPE
    //WE USE INTEGER CAUSE ITS THE BIGGEST SIZE
    retVec = et->getNewVector(it->getConsi32(INTEGER));
    et->initVector(retVec, rangeSize);
    retElmtsPtr = it->getPtrFromStruct(retVec, it->getConsi32(VEC_ELEM_INDEX));

    //push symbol table
    symbolTable->pushNewScope();

    //add loop var to scope
    symbolTable->addSymbol(loopVar, UNDEF, false, loopVarPtr);

    //welcome to my while statement
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    wb->beginWhile();
    //load loop var
    curIdx = ir->CreateLoad(curIdxPtr);

    //control
    wb->beginInsertControl();
    cond = ir->CreateICmpSLT(curIdx, rangeSize);
    wb->insertControl(cond);

    //get current index for the return
    curIdx = ir->CreateLoad(curIdxPtr, "curIdx");

    //set the loopvar
    curRangePtr = ir->CreateGEP(rangePtr, curIdx, "curRangePtr");
    curLoopVar  = ir->CreateLoad(curRangePtr, "loopVar");
    ir->CreateStore(curLoopVar, loopVarPtr);

    //get current value and its type
    curVal          = visit(exprNode);
    retElmtType     = curVal->getType();
    retElmtTypeCons = it->getConstFromType(retElmtType);

    //cast the void *  and get some addresses
    retElmtsPtr = ir->CreatePointerCast(retElmtsPtr, retElmtType->getPointerTo());
    retElmtPtr  = ir->CreateGEP(retElmtsPtr, curIdx);

    //prepare for call
    curValPtr = ir->CreatePointerCast(curValPtr, curVal->getType()->getPointerTo(), "curValPtr");
    ir->CreateStore(curVal, curValPtr);

    //store
    et->assignValFromPointers(retElmtPtr, curValPtr, retElmtTypeCons);

    //increment and store indices
    curIdx = ir->CreateAdd(curIdx, it->getConsi32(1));
    ir->CreateStore(curIdx, curIdxPtr);

    wb->endWhile();

    //skill the var scope
    symbolTable->popScope();

    //cast and return
    llvm::Value *retVecTypePtr = it->getPtrFromStruct(retVec, it->getConsi32(VEC_TYPE_INDEX));
    ir->CreateStore(retElmtTypeCons, retVecTypePtr);
    return it->castVectorToType(retVec, retElmtType);
}

/**
 *
 * @param rowLoopVar
 * @param colLoopVar
 * @param rowRange
 * @param colRange
 * @param exprNode
 * @return
 */
llvm::Value *
CodeGenerator::generateMatrix(std::string rowLoopVar, std::string colLoopVar, llvm::Value *rowRange, llvm::Value *colRange,
                              ASTNode *exprNode) {
    //variables for the return
    llvm::Value * retNumRows = it->getValFromStruct(rowRange, VEC_LEN_INDEX);
    llvm::Value * retNumCols = it->getValFromStruct(colRange, VEC_LEN_INDEX);
    llvm::Value * ret        = et->getNewMatrix(it->getConsi32(INTEGER));
    ret = ir->CreatePointerCast(ret, matrixTy->getPointerTo());
    et->initMatrix(ret, retNumRows, retNumCols);
    llvm::Value * retVecsPtr = it->getPtrFromStruct(ret, MATRIX_ELEM_INDEX);
    llvm::Value * retVecPtr  = nullptr;
    llvm::Value * retTypePtr = it->getPtrFromStruct(ret, MATRIX_TYPE_INDEX);
    llvm::Type  * retType    = nullptr;

    //variables to control the loop
    llvm::Value *rangePtr    = nullptr;
    llvm::Value *curRangePtr = nullptr;
    llvm::Value *rangeVecPtr = nullptr;
    llvm::Value *rangeSize   = nullptr;

    //variables for the current loop iteration
    llvm::Value *curIdx    = nullptr;
    llvm::Value *curIdxPtr = nullptr;
    llvm::Value *curVec    = nullptr;
    llvm::Value *curVecPtr = nullptr;
    llvm::Value *cond      = nullptr;

    //loop var
    llvm::Value *fixedLoopVarPtr = nullptr;
    llvm::Value *curLoopVar = nullptr;

    //visit the range, do the implicit by if needed, point to the integer elements
    rangeVecPtr = rowRange;
    rangePtr    = it->getPtrFromStruct(rangeVecPtr, it->getConsi32(VEC_ELEM_INDEX));
    rangeSize   = it->getValFromStruct(rangeVecPtr, it->getConsi32(VEC_LEN_INDEX));

    //init loop index
    curIdxPtr = ir->CreateAlloca(intTy);
    curIdx    = it->getConsi32(0);
    ir->CreateStore(curIdx, curIdxPtr);

    //add new scope
    symbolTable->pushNewScope();

    //add new loop var
    fixedLoopVarPtr = ir->CreateAlloca(rangePtr->getType()->getPointerElementType());
    symbolTable->addSymbol(rowLoopVar, UNDEF, false, fixedLoopVarPtr);

    auto * wb = new WhileBuilder(globalCtx, ir, mod);
    wb->beginWhile("MatrixGenerate");

    //load the index
    curIdx = ir->CreateLoad(curIdxPtr);

    //set the loopvar
    curRangePtr = ir->CreateGEP(rangePtr, curIdx, "curRangePtr");
    curLoopVar  = ir->CreateLoad(curRangePtr, "loopVar");
    ir->CreateStore(curLoopVar,fixedLoopVarPtr);

    //loop control
    wb->beginInsertControl();
    cond = ir->CreateICmpSLT(curIdx, retNumRows);
    wb->insertControl(cond);

    //load the index
    curIdx = ir->CreateLoad(curIdxPtr);

    //get the generated row
    curVecPtr = generateVector(colLoopVar, colRange, exprNode);

    //save the type
    retType = curVecPtr->getType();

    curVecPtr = ir->CreatePointerCast(curVecPtr, vecTy->getPointerTo());

    //get the current vector pointer
    retVecPtr = ir->CreateGEP(retVecsPtr, curIdx);

    //store the generated vector
    curVec = ir->CreateLoad(curVecPtr);
    ir->CreateStore(curVec, retVecPtr);

    //increment loop index
    curIdx = ir->CreateAdd(curIdx, it->getConsi32(1));
    ir->CreateStore(curIdx, curIdxPtr);

    wb->endWhile("EndMatrixGenerate");

    symbolTable->popScope();

    //Type handling
    ret = it->castMatrixToType(ret, retType);
    llvm::Value * consType = it->getConstFromType(retType);
    ir->CreateStore(consType, retTypePtr);

    return ret;
}

/**
 * Generates either a vector or a matrix
 * @param node
 * @return
 */
llvm::Value *CodeGenerator::visit(GeneratorNode *node) {
    //get pointers to the node values
    std::vector<ASTNode *>   *ranges   = node->getRanges();
    std::vector<std::string> *loopVars = node->getLoopVars();
    ASTNode                  *exprNode = node->getExprNode();

    //evaluate all ranges
    auto * rangeVec = new std::vector<llvm::Value *>;
    ASTNode *curRangeNode;
    llvm::Value * curRangeVec;

    for(uint i = 0; i < ranges->size(); i++){
        curRangeNode = ranges->at(i);
        curRangeVec = getRange(curRangeNode);
        rangeVec->push_back(curRangeVec);
    }

    //case single vector
    if(ranges->size() == 1)
        return generateVector(loopVars->at(0), rangeVec->at(0), exprNode);

    //case 2-dim matrix
    if(ranges->size() == 2)
        return generateMatrix(loopVars->at(0), loopVars->at(1), rangeVec->at(0), rangeVec->at(1), exprNode);

    return nullptr;
}

/**
 * performs the single filter of a vector
 * @param loopVar  - id of loop var
 * @param range    - vector of values
 * @param condNode - condition to pass filter
 * @return
 */
llvm::Value *CodeGenerator::filterVector(std::string loopVar, llvm::Value *range, ASTNode *condNode) {

    //variables for the return
    llvm::Value *retVec       = et->getOpResultVector(range, range);
    llvm::Value *retVecIdx    = it->getConsi32(0);
    llvm::Value *retVecIdxPtr = ir->CreateAlloca(intTy);
    llvm::Value *retElmtsPtr  = it->getPtrFromStruct(retVec, it->getConsi32(VEC_ELEM_INDEX));
    llvm::Value *retElmtPtr   = nullptr;
    llvm::Value *retLenPtr    = it->getPtrFromStruct(retVec, it->getConsi32(VEC_LEN_INDEX));
    ir->CreateStore(retVecIdx, retVecIdxPtr);

    //variables to control the loop
    llvm::Value *rangePtr    = nullptr; //GEP this
    llvm::Value *rangeVecPtr = nullptr;
    llvm::Value *rangeSize   = nullptr;

    //variables for the current loop iteration
    llvm::Value *curIdx    = nullptr;
    llvm::Value *curIdxPtr = nullptr;
    llvm::Value *curValPtr = nullptr;
    llvm::Value *curVal    = nullptr;
    llvm::Value *cond      = nullptr;

    //visit the range, do the implicit by if needed, point to the integer elements
    rangeVecPtr = range;
    rangePtr    = it->getPtrFromStruct(rangeVecPtr, VEC_ELEM_INDEX);
    rangeSize   = it->getValFromStruct(rangeVecPtr, VEC_LEN_INDEX);
    rangePtr->setName("RangeElm");

    //allocate space for the indices
    curIdxPtr = ir->CreateAlloca(intTy);
    curIdxPtr->setName("CurIdxPtr");

    //init the indices
    curIdx = it->getConsi32(0);
    ir->CreateStore(curIdx, curIdxPtr);

    //init the loop var to have proper type
    llvm::Value *loopVarPtr = ir->CreateAlloca(rangePtr->getType()->getPointerElementType());

    //push symbol table
    symbolTable->pushNewScope();

    //add loop var to scope
    symbolTable->addSymbol(loopVar, UNDEF, false, loopVarPtr);

    //welcome to my while statement
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    wb->beginWhile("FilterStart");
    //load loop var
    curIdx = ir->CreateLoad(curIdxPtr);
    curIdx->setName("curIdx");

    //control loop
    wb->beginInsertControl();
    wb->insertControl(ir->CreateICmpSLT(curIdx, rangeSize), "FilterControl");

    //get current indices
    curIdx = ir->CreateLoad(curIdxPtr);
    curIdx->setName("curIdx");

    //get current position in the range and current value
    curValPtr = ir->CreateGEP(rangePtr, curIdx);
    curValPtr->setName("CurValPtr");
    curVal    = ir->CreateLoad(curValPtr);
    ir->CreateStore(curVal, loopVarPtr);
    curVal->setName("CurVal");

    //eval cond
    cond = (visit(condNode));
    cond->setName("FilterCond");

    //if cond passes
    auto *cb = new CondBuilder(globalCtx, ir, mod);
    cb->beginIf(cond);

    //get result pointer
    retVecIdx  = ir->CreateLoad(retVecIdxPtr);
    retElmtPtr = ir->CreateGEP(retElmtsPtr, retVecIdx);

    //store val
    ir->CreateStore(curVal, retElmtPtr);

    //increment result index
    retVecIdx = it->getAdd(retVecIdx, it->getConsi32(1));
    ir->CreateStore(retVecIdx, retVecIdxPtr);

    cb->endIf();
    cb->finalize();

    //increment and store indices
    curIdx = it->getAdd(curIdx, it->getConsi32(1));
    ir->CreateStore(curIdx, curIdxPtr);

    wb->endWhile("EndFilter");

    symbolTable->popScope();

    //ret size
    retVecIdx  = ir->CreateLoad(retVecIdxPtr);
    ir->CreateStore(retVecIdx, retLenPtr);

    //return
    return retVec;
}

/**
 * Generates the filter.
 *  - gets vectors for each cond (c1, .., cn),
 *  - the last cond cn is ~(c1 v ... v cn-1)
 *  - then builds the tuples
 * @param node
 * @return
 */
llvm::Value *CodeGenerator::visit(FilterNode *node) {
    unsigned int i = 0;

    llvm::Value *range  = getRange(node->getRange());
    //llvm::Value *curVec = nullptr;
    auto *results       = new std::vector<llvm::Value *>;
    auto *tupTypes      = new std::vector<llvm::Type *>;
    std::string tupleID = "tuple";

    for(i = 0; i < node->getCondNodes()->size(); i++){
        llvm::Value *curVec = filterVector(node->getLoopVar(), range, node->getCondNodes()->at(i));
        results->push_back(curVec);
        tupTypes->push_back(curVec->getType()->getPointerTo());
        tupleID += "|" + it->getVectorTypeString(curVec);
    }

    llvm::StructType *structTy;

    //get tuple type
    if(symbolTable->resolveType(tupleID)){
        structTy = llvm::cast<llvm::StructType>(symbolTable->resolveType(tupleID)->getTypeDef());
    }
    else{
        structTy = llvm::StructType::create(*tupTypes, tupleID);
        symbolTable->addTupleType(structTy, new std::unordered_map<std::string, int>, tupTypes);
    }

    //make tuple
    llvm::Value *ret = ir->CreateAlloca(structTy);
    initTuple(ret, results);
    return ret;
}