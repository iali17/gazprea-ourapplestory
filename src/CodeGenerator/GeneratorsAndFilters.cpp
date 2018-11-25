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


llvm::Value *CodeGenerator::generateVector(std::string loopVar, ASTNode *range, ASTNode *exprNode){
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
    rangeVecPtr = getRange(range);
    rangePtr    = it->getPtrFromStruct(rangeVecPtr, it->getConsi32(VEC_ELEM_INDEX));
    rangeSize   = it->getValFromStruct(rangeVecPtr, it->getConsi32(VEC_LEN_INDEX));

    //allocate space for the indices
    curIdxPtr    = ir->CreateAlloca(intTy);
    retVecIdxPtr = ir->CreateAlloca(intTy);
    loopVarPtr   = ir->CreateAlloca(intTy);
    curValPtr    = ir->CreatePointerCast(ir->CreateAlloca(intTy), charTy->getPointerTo());

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
    symbolTable->addSymbol(loopVar, INTEGER, false, loopVarPtr);

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

llvm::Value *CodeGenerator::visit(GeneratorNode *node) {
    //get pointers to the node values
    std::vector<ASTNode *>   *ranges   = node->getRanges();
    std::vector<std::string> *loopVars = node->getLoopVars();
    ASTNode                  *exprNode = node->getExprNode();

    //init an object to hold all of the return vectors
    auto *vectors = new std::vector<llvm::Value *>;

    //loop vars
    unsigned int i = 0;
    llvm::Value * curVec = nullptr;

    //loop
    for(i = 0; i < ranges->size(); i++){
        curVec = generateVector(loopVars->at(i), ranges->at(i), exprNode);
        vectors->push_back(curVec);
    }

    //return if was only a vector
    if(ranges->size() <= 1) return curVec;

    //otherwise we need to make a new matrix

    return ASTBaseVisitor::visit(node);
}


llvm::Value *CodeGenerator::generateFilter(std::string loopVar, ASTNode *range, ASTNode *condNode) {

    //variables for the return
    llvm::Value *retVec       = nullptr;
    llvm::Value *retVecIdx    = nullptr;
    llvm::Value *retVecIdxPtr = nullptr;
    llvm::Value *retElmtsPtr  = nullptr;
    llvm::Value *retElmtPtr   = nullptr;

    //variables to control the loop
    llvm::Value *rangePtr    = nullptr; //GEP this
    llvm::Value *rangeVecPtr = nullptr;
    llvm::Value *rangeSize   = nullptr;

    //variables for the current loop iteration
    llvm::Value *curIdx    = nullptr;
    llvm::Value *curIdxPtr = nullptr;
    llvm::Value *curVal    = nullptr;
    llvm::Value *cond      = nullptr;

    //visit the range, do the implicit by if needed, point to the integer elements
    rangeVecPtr = getRange(range);
    rangePtr    = it->getPtrFromStruct(rangeVecPtr, it->getConsi32(VEC_ELEM_INDEX));
    rangeSize   = it->getValFromStruct(rangeVecPtr, it->getConsi32(VEC_LEN_INDEX));

    //allocate space for the indices
    curIdxPtr    = ir->CreateAlloca(intTy);
    retVecIdxPtr = ir->CreateAlloca(intTy);

    //init the indices
    curIdx    = it->getConsi32(0);
    retVecIdx = it->getConsi32(0);
    ir->CreateStore(curIdx, curIdxPtr);
    ir->CreateStore(retVecIdx, retVecIdxPtr);

    //init the return

    //welcome to my while statement
    auto *wb = new WhileBuilder(globalCtx, ir, mod);
    wb->beginWhile();
    //load loop var

    //control loop
    wb->beginInsertControl();
    //
    wb->insertControl(cond);

    //get current indices

    //get current value

    //store

    //increment and store indices

    wb->endWhile();

    return retVec;
}
