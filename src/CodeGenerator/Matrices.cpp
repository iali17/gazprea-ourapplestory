
//
// Created by kyle on 20/11/18.
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
 *
 * @param node
 * @return - pointer to the new matrix
 */
llvm::Value *CodeGenerator::visit(MatrixNode *node) {
    //local vars

    //vector nodes and llvm pointers
    auto *vectorNodes   = node->getElements();
    auto *vectors       = new std::vector<llvm::Value *>;
    ASTNode *vectorNode = nullptr;
    llvm::Value *vPtr   = nullptr;

    //locals for matrix setup
    llvm::Value *numCols    = nullptr;
    llvm::Value *numRows    = nullptr;
    llvm::Value *numColsPtr = nullptr;
    llvm::Value *curCols    = nullptr;
    llvm::Value *consTy     = nullptr;
    llvm::Type  *vecTy      = nullptr;

    //return value
    llvm::Value *matrix = nullptr;

    //this code will not work for a completely empty matrix so
    assert(not(vectorNodes->empty()));

    //we will keep track of the longest vector in here
    numColsPtr = ir->CreateAlloca(intTy);
    ir->CreateStore(it->getConsi32(0), numColsPtr);

    //get vectors
    for(uint i = 0; i < vectorNodes->size(); i++){
        vectorNode = vectorNodes->at(i);
        vPtr       = visit(vectorNode);
        vectors->push_back(vPtr);

        //set the max num cols

        //get the column lengths to compare
        curCols = it->getValFromStruct(vPtr, it->getConsi32(1));
        numCols = ir->CreateLoad(numColsPtr);

        //store col length if max
        auto *cb = new CondBuilder(globalCtx, ir, mod);
        cb->beginIf(ir->CreateICmpSGT(curCols, numCols));
            ir->CreateStore(curCols, numColsPtr);
        cb->endIf();
        cb->finalize();
    }

    //now we know the max col size and the row size
    numCols = ir->CreateLoad(numColsPtr);
    numRows = it->getConsi32(vectorNodes->size());

    //init matrix

    //find the type and base on the first element
    vecTy   = it->getVectorElementType(vectors->at(0));
    consTy  = it->getConstFromType(vecTy);

    //create new matrix and initialize to the size
    matrix  = et->getNewMatrix(consTy);
    et->initMatrix(matrix, numRows, numCols);

    //assign all of the matrix values
    for(uint i = 0; i < vectorNodes->size(); i++){
        //TODO - write an internal tools function to assign vector values to another THAT ALSO PERFORMS IMPLICIT UPCASTING
    }

    return it->castMatrixToType(matrix, vecTy);
}

llvm::Value *CodeGenerator::visit(MatrixDeclNode *node) {
    MatrixNode *matrixNode = nullptr;
    auto *matrixType = dynamic_cast<MatrixType *>(node->getMatrixType());

    matrixNode = dynamic_cast<MatrixNode *>(node->getExpr());

  /*
    std::string stype = vectorType->getStringType();
    llvm::Type *type = it->getVectorType(stype);
    llvm::Value *size = visit(node->getSize());

    llvm::Value *vec = et->getNewVector(it->getConstFromType(type));
    vec = it->castVectorToType(vec, type);

    if (size){
        et->initVector(vec, size);
    }

    if (dynamic_cast<IdnNode *>(node->getExpr())) {
        et->setIdentityVector(vec);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), vec);
        return nullptr;
    } else if (dynamic_cast<NullNode *>(node->getExpr())) {
        et->setNullVector(vec);
        symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), vec);
        return nullptr;
    }

    if (vectorNode) {
        if (!vectorNode->getElements()->empty()) {
            llvm::Value *vecExpr = visit(node->getExpr());
            llvm::Type *vecType = it->getDeclVectorType(stype);
            llvm::Value *vecExprSize = it->getValFromStruct(vecExpr, it->getConsi32(VEC_LEN_INDEX));

            vec = ct->typeAssCast(vecType, vecExpr, node->getLine(), size, (int)vectorNode->getElements()->size());
        }
            // empty vector
        else  {
            et->initVector(vec, it->getConsi32(0));
        }

    }
        // Handles case when expr is not a vector -> scalar, haven't tested tuple. Pretty sure segfaults if tested
    else if (node->getExpr()) {

        llvm::Value *regExpr = visit(node->getExpr());
        llvm::Type *vecType = it->getDeclVectorType(stype);

        vec = ct->typeAssCast(vecType, regExpr, node->getLine(), size);
    }

    symbolTable->addSymbol(node->getID(), node->getType(), node->isConstant(), vec);

    */
    return nullptr;
}

llvm::Value *CodeGenerator::indexMatrix(llvm::Value *mat, llvm::Value *rowIdx, llvm::Value *colIdx, bool isSlice) {
    return nullptr;
}


llvm::Value *CodeGenerator::matrixSliceAssign(IndexNode *idxExpr, llvm::Value *dest, std::vector<llvm::Value *> *idxVec,
                                              llvm::Value *src) {
    return nullptr;
}
