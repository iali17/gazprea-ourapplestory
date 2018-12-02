//
// Created by kyle on 20/10/18.
//

#ifndef VCALCBASE_EXTERNALTOOLS_H
#define VCALCBASE_EXTERNALTOOLS_H

#include <vector>
#include <string>
#include "llvm/IR/Value.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/IR/Verifier.h"
#include "InternalTools/InternalTools.h"

#define INT_FORMAT_STR   "intFormatStr"
#define BOOL_FORMAT_STR  "charFormatStr"
#define CHAR_FORMAT_STR  "charFormatStr"
#define FLOAT_FORMAT_STR "floatFormatStr"
#define EOLN_STR         "eolnStr"
#define SPACE_STR        "spaceStr"
#define OPEN_SQRB_STR    "openSqrBStr"
#define CLOSE_SQRB_STR   "closeSqrBStr"
#define TRUE_STR         "T"
#define FALSE_STR        "F"

class ExternalTools {
public:
    ExternalTools(llvm::LLVMContext *globalCtx, llvm::IRBuilder<> *ir, llvm::Module *mod);

    //calloc
    void registerCalloc();
    llvm::Value * aliCalloc(llvm::Value * arrSize, int elementSize = 4, bool castToIntP = true);

    //pow
    void registerPow();
    llvm::Value * aliPow(llvm::Value *left, llvm::Value *right);

    //free
    void registerFree();
    void aliFree(llvm::Value * p);

    //scanf
    void registerScanf();
    llvm::Value *aliScanf(llvm::Value *stream, llvm::Value *scanTo, llvm::Value *type);

    //vector functions
    void registerVectorFunctions();
    llvm::Value * getNewVector(llvm::Value *ty);
    llvm::Value * initVector(llvm::Value *vec, llvm::Value *size);
    llvm::Value * setNullVector(llvm::Value * vec);
    llvm::Value * setIdentityVector(llvm::Value * vec);
    llvm::Value * getVectorLength(llvm::Value *vec);
    llvm::Value * getReverseVector(llvm::Value *fromVec);
    llvm::Value * printVector(llvm::Value *vec);
    llvm::Value * printVectorElement(llvm::Value * vecElmPtr, llvm::Value * type);
    llvm::Value * copyVectorElements(llvm::Value * dest, llvm::Value * src);
    llvm::Value * strictCopyVectorElements(llvm::Value * dest, llvm::Value * src, llvm::Value * line);
    llvm::Value * getVectorSlice(llvm::Value *vec, llvm::Value *idxVec);
    llvm::Value * getVectorCopy(llvm::Value * vec);
    llvm::Value * getVectorElementPointer(llvm::Value * vec, llvm::Value * idx);
    llvm::Value * getVectorElementPointerSafe(llvm::Value * vec, llvm::Value *idx);
    llvm::Value * getDotProduct(llvm::Value *leftVec, llvm::Value *rightVec);
    llvm::Value * assignValFromPointers(llvm::Value * left, llvm::Value *right, llvm::Value *ty);
    llvm::Value * getOpResultVector(llvm::Value *left, llvm::Value *right);
    llvm::Value * concatenateVectors(llvm::Value * left,llvm::Value * right);
    llvm::Value * getVectorBy(llvm::Value * vec, llvm::Value * by);
    llvm::Value * assignFromVector(llvm::Value * dest, llvm::Value * idx, llvm::Value * src);

    //Interval functions
    void registerIntervalFunctions();
    llvm::Value * getNewInterval(llvm::Value * lower, llvm::Value * upper);
    llvm::Value * getVectorFromInterval(llvm::Value *interval, llvm::Value * by);

    //matrix functions
    void registerMatrixFunctions();
    llvm::Value * getNewMatrix(llvm::Value *ty);
    llvm::Value * initMatrix(llvm::Value *mat, llvm::Value * numRows, llvm::Value *numCols);
    llvm::Value * setNullMatrix(llvm::Value * mat);
    llvm::Value * setIdentityMatrix(llvm::Value *mat);
    llvm::Value * getNumRows(llvm::Value *mat);
    llvm::Value * getNumCols(llvm::Value *mat);
    llvm::Value * printMatrix(llvm::Value *mat);
    llvm::Value * sliceMatrix(llvm::Value *mat, llvm::Value *l, llvm::Value *r);
    llvm::Value * indexMatrix(llvm::Value *mat, llvm::Value *l, llvm::Value *r);
    llvm::Value * getMatrixElementPointer(llvm::Value *mat, llvm::Value *row, llvm::Value *col);
    llvm::Value * getMatrixMultiplication(llvm::Value *leftMat, llvm::Value *rightMat);
    llvm::Value * concatenateMatrices(llvm::Value * left, llvm::Value * right);
    llvm::Value * assignScalarVector(llvm::Value * matrix, llvm::Value * scalar, llvm::Value * vector, llvm::Value * src);
    llvm::Value * assignVectorScalar(llvm::Value * matrix,llvm::Value * vector, llvm::Value * scalar,llvm::Value * src);
    llvm::Value * assignVectorVector(llvm::Value * matrix, llvm::Value * vectorRow, llvm::Value * vectorCol, llvm::Value * src);
    llvm::Value * strictCopyMatrixElements(llvm::Value * dest, llvm::Value * src, llvm::Value * line);

    //printing
    void registerPrintf();
    void printStaticStr(std::string constStr);
    void printInt(llvm::Value * val);
    void printReal(llvm::Value * val);
    void printStr(llvm::Value *val);
    void printChar(llvm::Value *val);
    void printBoolean(llvm::Value *val);
    void print(llvm::Value * val);

protected:
    llvm::LLVMContext * globalCtx;
    llvm::IRBuilder<> * ir;
    llvm::Module      * mod;

    //matrix slicing
    llvm::Value * sliceScalarVector(llvm::Value * mat, llvm::Value * scalar, llvm::Value *vec);
    llvm::Value * sliceVectorScalar(llvm::Value * mat, llvm::Value * vec,    llvm::Value *scalar);
    llvm::Value * sliceVectorVector(llvm::Value * mat, llvm::Value * vecRow, llvm::Value *vecCol);

    //matrix indexing
    llvm::Value * indexScalarVector(llvm::Value * mat, llvm::Value * scalar, llvm::Value *vec);
    llvm::Value * indexVectorScalar(llvm::Value * mat, llvm::Value * vec,    llvm::Value *scalar);
    llvm::Value * indexVectorVector(llvm::Value * mat, llvm::Value * vecRow, llvm::Value *vecCol);

    //matrix multiplication
    llvm::Value * getIntMatrixMultiplication(llvm::Value *leftMat, llvm::Value *rightMat);
    llvm::Value * getRealMatrixMultiplication(llvm::Value *leftMat, llvm::Value *rightMat);

    //dot product
    llvm::Value * getIntDotProduct(llvm::Value *leftVec, llvm::Value *rightVec);
    llvm::Value * getRealDotProduct(llvm::Value *leftVec, llvm::Value *rightVec);
};
#endif //VCALCBASE_EXTERNALTOOLS_H
