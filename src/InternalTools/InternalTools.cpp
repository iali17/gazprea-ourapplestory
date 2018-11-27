//
// Created by kyle on 20/10/18.
//

#include <InternalTools/InternalTools.h>

extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;
extern llvm::Type *realTy;
extern llvm::Type *boolTy;
extern llvm::Type *strTy;
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
extern llvm::Type *streamStateTy;

InternalTools::pair InternalTools::makePair(llvm::Value *leftV, llvm::Value *rightV) {
    pair pair1;
    pair1.left = leftV;
    pair1.right = rightV;
    return pair1;
}

/**
 *	returns the actual value
 */
llvm::Value *InternalTools::getValFromTuple(llvm::Value *tuplePtr, llvm::Value *idx) {
	llvm::Value *ptr = ir->CreateInBoundsGEP(tuplePtr, {getConsi32(0), idx});
	llvm::Value *val = ir->CreateLoad(ptr);
	return ir->CreateLoad(val);
}


/**
 *	returns a pointer to the the tuple element. YOu can store to here
 */
llvm::Value *InternalTools::getPtrFromTuple(llvm::Value *tuplePtr, llvm::Value *idx){
    llvm::Value *ptr = ir->CreateInBoundsGEP(tuplePtr, {getConsi32(0), idx});
    return ir->CreateLoad(ptr);
}

llvm::Value *InternalTools::getReal(float val) {
    return llvm::ConstantFP::get(realTy, val);
}

llvm::Value *InternalTools::geti1(int64_t val) {
    uint64_t trueValue = static_cast<uint64_t>(static_cast<int64_t>(val));
    return llvm::ConstantInt::get(boolTy, trueValue, true);
}

llvm::Value *InternalTools::geti8(int64_t val) {
    uint64_t trueValue = static_cast<uint64_t>(static_cast<int64_t>(val));
    return llvm::ConstantInt::get(i8Ty, trueValue, true);
}

llvm::Value *InternalTools::getConsi32(int64_t val) {
    uint64_t trueValue = static_cast<uint64_t>(static_cast<int64_t>(val));
    return llvm::ConstantInt::get(i32Ty, trueValue, true);
}

llvm::Value *InternalTools::getConsi64(int64_t val) {
    uint64_t trueValue = static_cast<uint64_t>(static_cast<int64_t>(val));
    return llvm::ConstantInt::get(i64Ty, trueValue, true);
}

bool InternalTools::setNull(llvm::Type * type, llvm::Value * ptr) {
    if(type == boolTy){
        ir->CreateStore(geti1(0), ptr);
        return true;
    }
    else if (type == i8Ty){
        ir->CreateStore(geti8(0), ptr);
        return true;
    }
    else if(type == intTy){
        ir->CreateStore(getConsi32(0), ptr);
        return true;
    }
    else if(type == realTy){
        ir->CreateStore(getReal(0), ptr);
        return true;
    }
    return false;
}

InternalTools::InternalTools(llvm::LLVMContext *globalCtx, llvm::IRBuilder<> *ir, llvm::Module *mod) : globalCtx(
        globalCtx), ir(ir), mod(mod) {
    setUpTypes();
}

llvm::Value *InternalTools::safeReplace(llvm::Value *safePtr, llvm::Value *cond, llvm::Value *idx, llvm::Value *arrP,
                                        llvm::Value *replaceVal) {
    llvm::Function *curFunction = ir->GetInsertBlock()->getParent();
    llvm::BasicBlock *thenBB    = llvm::BasicBlock::Create(*globalCtx, "then", curFunction);
    llvm::BasicBlock *elseBB    = llvm::BasicBlock::Create(*globalCtx, "else");
    llvm::BasicBlock *mergeBB   = llvm::BasicBlock::Create(*globalCtx, "ifcont");

    //eval condition
    ir->CreateCondBr(cond, thenBB, elseBB);

    ir->SetInsertPoint(thenBB);
    llvm::Value *ptr = ir->CreateGEP(arrP, idx);
    llvm::Value *val = ir->CreateLoad(ptr, "val");
    ir->CreateStore(val, safePtr);
    ir->CreateBr(mergeBB);

    curFunction->getBasicBlockList().push_back(elseBB);
    ir->SetInsertPoint(elseBB);
    ir->CreateStore(replaceVal, safePtr);
    ir->CreateBr(mergeBB);

    curFunction->getBasicBlockList().push_back(mergeBB);
    ir->SetInsertPoint(mergeBB);
    return nullptr;
}

void InternalTools::setUpTypes() {
    i64Ty  = llvm::TypeBuilder<llvm::types::i<64>, true>::get(*globalCtx);
    i32Ty  = llvm::TypeBuilder<llvm::types::i<32>, true>::get(*globalCtx);
    intTy  = llvm::TypeBuilder<llvm::types::i<32>, true>::get(*globalCtx);
    i8Ty   = llvm::TypeBuilder<llvm::types::i<8>,  true>::get(*globalCtx);
    charTy = llvm::TypeBuilder<llvm::types::i<8>,  true>::get(*globalCtx);
    boolTy = llvm::TypeBuilder<llvm::types::i<1>,  true>::get(*globalCtx);
    realTy = llvm::Type::getFloatTy(*globalCtx);

    //generic types (void *)
    //vector type
    std::vector<llvm::Type *> types;
    types.push_back(intTy->getPointerTo());
    types.push_back(intTy->getPointerTo());
    types.push_back(charTy->getPointerTo());
    vecTy = llvm::StructType::create(*globalCtx, types, "vector");

    //matrix type
    std::vector<llvm::Type *> mTypes;
    mTypes.push_back(intTy->getPointerTo());
    mTypes.push_back(intTy->getPointerTo());
    mTypes.push_back(intTy->getPointerTo());
    mTypes.push_back(vecTy->getPointerTo()->getPointerTo());
    matrixTy = llvm::StructType::create(*globalCtx, mTypes, "matrix");

    //INTEGER
    //vector type
    types.back() = intTy->getPointerTo();
    intVecTy = llvm::StructType::create(*globalCtx, types, "intVector");

    //matrix type
    mTypes.back() = intVecTy->getPointerTo();
    intMatrixTy = llvm::StructType::create(*globalCtx, mTypes, "intMatrix");

    //CHAR
    //vector type
    types.back() = charTy->getPointerTo();
    charVecTy = llvm::StructType::create(*globalCtx, types, "charVector");

    //quickly do string as well
    strTy = llvm::StructType::create(*globalCtx, types, "string");

    //matrix type
    mTypes.back() = charVecTy->getPointerTo();
    charMatrixTy = llvm::StructType::create(*globalCtx, mTypes, "charMatrix");

    //BOOL
    //vector type
    types.back() = boolTy->getPointerTo();
    boolVecTy = llvm::StructType::create(*globalCtx, types, "boolVector");

    //matrix type
    mTypes.back() = boolVecTy->getPointerTo();
    boolMatrixTy = llvm::StructType::create(*globalCtx, mTypes, "boolMatrix");

    //REAL
    //vector type
    types.back() = realTy->getPointerTo();
    realVecTy = llvm::StructType::create(*globalCtx, types, "realVector");

    //matrix type
    mTypes.back() = realVecTy->getPointerTo();
    realMatrixTy = llvm::StructType::create(*globalCtx, mTypes, "realMatrix");

    //interval type
    std::vector<llvm::Type *> iTypes;
    iTypes.push_back(intTy->getPointerTo());
    iTypes.push_back(intTy->getPointerTo());
    intervalTy = llvm::StructType::create(*globalCtx, iTypes, "interval");

    //stream state
    std::vector<llvm::Type *> sTypes;
    sTypes.push_back(intTy);
    sTypes.push_back(intTy);
    streamStateTy = llvm::StructType::create(*globalCtx, sTypes, "streamState");
}

llvm::Value *InternalTools::getNull(llvm::Type *type) {
    if(type == boolTy)
        return geti1(0);
    else if(type == charTy)
        return geti8(0);
    else if(type == intTy)
        return getConsi32(0);
    else if(type == realTy)
        return getReal(0.0);
    else
        return nullptr;
}

llvm::Value *InternalTools::getIdn(llvm::Type *type) {
    if(type == boolTy)
        return geti1(1);
    else if(type == charTy)
        return geti8(1);
    else if(type == intTy)
        return getConsi32(1);
    else if(type == realTy)
        return getReal(1.0);
    else
        return nullptr;
}

llvm::Value *InternalTools::initTuple(llvm::Value *tuplePtr, std::vector<llvm::Value *> *values) {
    //fill new structure
    auto *structType = llvm::cast<llvm::StructType>(tuplePtr->getType()->getPointerElementType());
    auto types   = structType->elements();
    llvm::Value *element;
    llvm::Type * nType, * oType;
    for(unsigned long i = 0; i < values->size(); ++i){
        llvm::Value *structElem = ir->CreateInBoundsGEP(tuplePtr, {getConsi32(0), getConsi32(i)});
        llvm::Value *ptr        = ir->CreateAlloca(types[i]->getPointerElementType());

        nType = types[i]->getPointerElementType();
        oType = values->at(i)->getType();
        element = values->at(i);
        if((nType != oType) && (nType == realTy)&& (oType == intTy))
            element = ir->CreateSIToFP(element, realTy);

        ir->CreateStore(element, ptr);
        ir->CreateStore(ptr, structElem);
    }
    return tuplePtr ;
}

llvm::Value *InternalTools::initTupleFromPtrs(llvm::Value *tuplePtr, std::vector<llvm::Value *> *ptrs) {
    for(unsigned long i = 0; i < ptrs->size(); ++i){
        llvm::Value *structElem = ir->CreateInBoundsGEP(tuplePtr, {getConsi32(0), getConsi32(i)});
        llvm::Value *ptr        = ptrs->at(i);
        assert(ptr->getType() == structElem->getType()->getPointerElementType());
        ir->CreateStore(ptr, structElem);
    }
    return tuplePtr ;
}

std::vector<llvm::Value *> *InternalTools::getValueVectorFromTuple(llvm::Value *structPtr) {
    auto *structType = llvm::cast<llvm::StructType>(structPtr->getType()->getPointerElementType());
    auto types   = structType->elements();
    auto * values = new std::vector<llvm::Value *>;

    for(unsigned long i = 0; i < types.size(); ++i){
        llvm::Value *structElem = getValFromTuple(structPtr, getConsi32(i));
        values->push_back(structElem);
    }
    return values;
}

std::vector<llvm::Value *> *InternalTools::getPtrVectorFromStruct(llvm::Value *structPtr) {
    auto *structType = llvm::cast<llvm::StructType>(structPtr->getType()->getPointerElementType());
    auto types   = structType->elements();
    auto * values = new std::vector<llvm::Value *>;

    for(unsigned long i = 0; i < types.size(); ++i){
        llvm::Value *structElem = getPtrFromTuple(structPtr, getConsi32(i));
        values->push_back(structElem);
    }
    return values;
}

llvm::Value *InternalTools::getAdd(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateAdd(left, right, "iaddtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFAdd(left, right, "faddtmp");
    }

    std::cerr << "Ambiguous types during arithmetic operation\n";
    return nullptr;
}

llvm::Value *InternalTools::getSub(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateSub(left, right, "iaddtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFSub(left, right, "faddtmp");
    }

    std::cerr << "Ambiguous types during arithmetic operation\n";
    exit(1);
}

llvm::Value *InternalTools::getMul(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateMul(left, right, "imultmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFMul(left, right, "fmultmp");
    }

    std::cerr << "Ambiguous types during arithmetic operation\n";
    exit(1);}

llvm::Value *InternalTools::getDiv(llvm::Value *left, llvm::Value *right) {
    if (right == this->getConsi32(0) || right == this->getReal(0)) {
        std::cerr <<"Division by zero, Aborting.....\n";
        exit(1);
    }

    if(left->getType() == intTy){
        return ir->CreateSDiv(left, right, "idivtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFDiv(left, right, "fdivtmp");
    }

    std::cerr << "Ambiguous types during arithmetic operation\n";
    exit(1);
}

llvm::Value *InternalTools::getRem(llvm::Value *left, llvm::Value *right) {
    if (right == this->getConsi32(0) || right == this->getReal(0)) {
        std::cerr <<"Modulo by zero, Aborting.....\n";
        exit(1);
    }

    if(left->getType() == intTy){
        return ir->CreateSRem(left, right, "iremtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFRem(left, right, "fremtmp");
    }

    std::cerr << "Ambiguous types during arithmetic operation\n";
    exit(1);
}

llvm::Value *InternalTools::getEQ(llvm::Value *left, llvm::Value *right) {
    if (left->getType() == boolTy){
        return ir->CreateICmpEQ(left, right, "fuck");
    }
    else if(left->getType() == intTy){
        return ir->CreateICmpEQ(left, right, "ieqtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpUEQ(left, right, "feqtmp");
    }

    std::cerr << "Ambiguous types during arithmetic operation\n";
    exit(1);
}

llvm::Value *InternalTools::getNEQ(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == boolTy){
        return ir->CreateICmpNE(left, right, "ineqtmp");
    }
    if(left->getType() == intTy){
        return ir->CreateICmpNE(left, right, "ineqtmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpUNE(left, right, "fneqtmp");
    }

    std::cerr << "Ambiguous types during arithmetic operation\n";
    exit(1);
}

llvm::Value *InternalTools::getGT(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateICmpSGT(left, right, "igttmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpUGT(left, right, "fgttmp");
    }

    std::cerr << "Ambiguous types during arithmetic operation\n";
    exit(1);
}

llvm::Value *InternalTools::getLT(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateICmpSLT(left, right, "ilttmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpULT(left, right, "flttmp");
    }

    std::cerr << "Ambiguous types during arithmetic operation\n";
    exit(1);
}

llvm::Value *InternalTools::getLTE(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateICmpSLE(left, right, "iltetmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpOLE(left, right, "fltetmp");
    }

    std::cerr << "Ambiguous types during arithmetic operation\n";
    exit(1);
}

llvm::Value *InternalTools::getGTE(llvm::Value *left, llvm::Value *right) {
    if(left->getType() == intTy){
        return ir->CreateICmpSGE(left, right, "igtetmp");
    }
    else if(left->getType() == realTy){
        return ir->CreateFCmpOGE(left, right, "fgtetmp");
    }

    std::cerr << "Ambiguous types during arithmetic operation\n";
    exit(1);
}

llvm::Value *InternalTools::getAnd(llvm::Value *left, llvm::Value *right) {
    return ir->CreateAnd(left, right, "andtmp");
}

llvm::Value *InternalTools::getOr(llvm::Value *left, llvm::Value *right) {
    return ir->CreateOr(left, right, "andtmp");
}

llvm::Value *InternalTools::getXOr(llvm::Value *left, llvm::Value *right) {
    return ir->CreateXor(left, right, "andtmp");
}

llvm::Value *InternalTools::getNegation(llvm::Value *expr) {
    assert(expr->getType() == boolTy);
    return ir->CreateNot(expr, "negtmp");
}

llvm::Value *InternalTools::getUnarySub(llvm::Value *expr) {
    if (expr->getType() == realTy){
        return ir->CreateFNeg(expr, "fnegtmp");
    }

    return ir->CreateNeg(expr, "negtmp");
}

bool InternalTools::isStructType(llvm::Value *ptr) {
    if(ptr->getType()->isPointerTy()){
        return ptr->getType()->getPointerElementType()->isStructTy();
    }
    return false;
}

bool InternalTools::isTupleType(llvm::Value *ptr) {
    if (isVectorType(ptr) || isMatrixType(ptr) || isIntervalType(ptr)){
        return false;
    }
    else if(ptr->getType()->isPointerTy()){
        return ptr->getType()->getPointerElementType()->isStructTy();
    }
    return false;
}

bool InternalTools::isIntervalType(llvm::Value *ptr) {
    return  ptr->getType() == intervalTy->getPointerTo();
}

bool InternalTools::isVectorType(llvm::Value *ptr) {
    if(ptr->getType() == intVecTy->getPointerTo()) {
        return true;
    } else if(ptr->getType() == realVecTy->getPointerTo()) {
        return true;
    } else if(ptr->getType() == charVecTy->getPointerTo()) {
        return true;
    } else if(ptr->getType() == boolVecTy->getPointerTo()) {
        return true;
    } else if(ptr->getType() == strTy->getPointerTo()) {
        return true;
    }

    return false;
}


bool InternalTools::isMatrixType(llvm::Value *ptr) {
    if(ptr->getType() == intMatrixTy->getPointerTo()) {
        return true;
    } else if(ptr->getType() == realMatrixTy->getPointerTo()) {
        return true;
    } else if(ptr->getType() == charMatrixTy->getPointerTo()) {
        return true;
    } else if(ptr->getType() == boolMatrixTy->getPointerTo()) {
        return true;
    }

    return false;
}

bool InternalTools::isDeclVectorType(llvm::Type *type) {
    if(type == intVecTy || type == intVecTy->getPointerTo()) {
        return true;
    } else if(type == realVecTy || type == realVecTy->getPointerTo()) {
        return true;
    } else if(type == charVecTy || type == charVecTy->getPointerTo()) {
        return true;
    } else if(type == boolVecTy || type == boolVecTy->getPointerTo()) {
        return true;
    } else if(type == strTy || type == strTy->getPointerTo()) {
        return true;
    }

    return false;
}


std::string InternalTools::getType(llvm::Type *type, llvm::Value *expr) {
    if(type == intTy)
        return "integer";
    else if(type == realTy)
        return "real";
    else if(type == boolTy)
        return "boolean";
    else if(type == charTy)
        return "character";
    else if(isStructType(expr))
        return "tuple(*)";
    else
        return nullptr;
}

/**
 * get the integer constant representation of the type for vector and matrix generation
 *
 * @param ty
 * @return
 */
llvm::Value *InternalTools::getConstFromType(llvm::Type *ty) {
    if(ty == boolTy || ty == boolVecTy->getPointerTo())
        return getConsi32(BOOLEAN);
    else if (ty == charTy || ty == charVecTy->getPointerTo())
        return getConsi32(CHAR);
    else if (ty == intTy || ty == intVecTy->getPointerTo())
        return getConsi32(INTEGER);
    else if (ty == realTy || ty == realVecTy->getPointerTo())
        return getConsi32(REAL);

    return nullptr;
}

llvm::Type *InternalTools::getVectorElementType(llvm::Value *vec) {
    llvm::Value * ptr = getPtrFromStruct(vec, getConsi32(2));
    return ptr->getType()->getPointerElementType();
}

llvm::Value *InternalTools::getValFromStruct(llvm::Value *sPtr, llvm::Value *idx) {
    llvm::Value *ptr = ir->CreateInBoundsGEP(sPtr, {getConsi32(0), idx});
    llvm::Value *val = ir->CreateLoad(ptr);
    return ir->CreateLoad(val);
}

llvm::Value *InternalTools::getPtrFromStruct(llvm::Value *sPtr, llvm::Value *idx) {
    llvm::Value *ptr = ir->CreateInBoundsGEP(sPtr, {getConsi32(0), idx});
    return ir->CreateLoad(ptr);
}

llvm::Value *InternalTools::castVectorToType(llvm::Value *vec, llvm::Type *type) {
    if(type == boolTy)
        return ir->CreatePointerCast(vec, boolVecTy->getPointerTo());
    else if (type == charTy)
        return ir->CreatePointerCast(vec, charVecTy->getPointerTo());
    else if (type == intTy)
        return ir->CreatePointerCast(vec, intVecTy->getPointerTo());
    else if (type == realTy)
        return ir->CreatePointerCast(vec, realVecTy->getPointerTo());

    return nullptr;
}

llvm::Value *InternalTools::setVectorValues(llvm::Value *vec, std::vector<llvm::Value *> *values) {
    llvm::Value *ptr = getPtrFromStruct(vec, getConsi32(2));
    llvm::Value *curPosPtr;

    for(uint i = 0; i < values->size(); i++) {
        curPosPtr = ir->CreateGEP(ptr, getConsi32(i));
        ir->CreateStore(values->at(i), curPosPtr);
    }

    return nullptr;
}


llvm::Value *InternalTools::setMatrixValues(llvm::Value *mat, std::vector<llvm::Value *> *values) {
    llvm::Value *ptr = getPtrFromStruct(mat, getConsi32(MATRIX_ELEM_INDEX));
    llvm::Value *curRowPtr;

    for(uint i = 0; i < values->size(); i++) {
        curRowPtr = ir->CreateGEP(ptr, getConsi32(i));
        ir->CreateStore(values->at(i), curRowPtr);
    }

    return nullptr;
}

/**
 * Either pass the element type or the vector type
 * @param mat
 * @param type
 * @return a cast of the matrix pointer to the destination type
 */
llvm::Value *InternalTools::castMatrixToType(llvm::Value *mat, llvm::Type *type) {
    if((type == boolTy) || (type == boolVecTy->getPointerTo()))
        return ir->CreatePointerCast(mat, boolMatrixTy->getPointerTo());
    else if ((type == charTy) || (type == charVecTy->getPointerTo()))
        return ir->CreatePointerCast(mat, charMatrixTy->getPointerTo());
    else if ((type == intTy)  || (type == intVecTy->getPointerTo()))
        return ir->CreatePointerCast(mat, intMatrixTy->getPointerTo());
    else if ((type == realTy) || (type == realVecTy->getPointerTo()))
        return ir->CreatePointerCast(mat, realMatrixTy->getPointerTo());

    return nullptr;
}

llvm::Type *InternalTools::getVectorType(const std::string &typeString) {
    if(typeString == "integervector")
        return intTy;
    else if(typeString == "realvector")
        return realTy;
    else if(typeString == "booleanvector")
        return boolTy;
    else if(typeString == "charactervector")
        return charTy;
    else if(typeString == "string")
        return strTy;

    return nullptr;
}

llvm::Type *InternalTools::getDeclVectorType(const std::string &typeString) {
    if(typeString == "integervector")
        return intVecTy;
    else if(typeString == "realvector")
        return realVecTy;
    else if(typeString == "booleanvector")
        return boolVecTy;
    else if(typeString == "charactervector")
        return charVecTy;
    else if(typeString == "string")
        return strTy;

    return nullptr;
}

std::string InternalTools::getVectorTypeString(llvm::Value *vec) {
    std::string ret = "";
    if(vec->getType() == intVecTy->getPointerTo())
        ret = "integervector";
    else if(vec->getType() == realVecTy->getPointerTo())
       ret = "realvector";
    else if(vec->getType() == boolVecTy->getPointerTo())
        ret = "booleanvector";
    else if(vec->getType() == charVecTy->getPointerTo())
        ret = "charactervector";
    else if(vec->getType() == charVecTy->getPointerTo())
        ret = "string";
    return ret;
}

llvm::Type *InternalTools::getDeclScalarTypeFromVec(llvm::Type *type) {
    if(type == intVecTy || type == intVecTy->getPointerTo()) {
        return intTy;
    } else if(type == realVecTy || type == realVecTy->getPointerTo()) {
        return realTy;
    } else if((type == charVecTy) || (type == strTy) || type == charVecTy->getPointerTo() || type == strTy->getPointerTo()) {
        return charTy;
    } else if(type == boolVecTy || type == boolVecTy->getPointerTo()) {
        return boolTy;
    }

    return nullptr;
}
