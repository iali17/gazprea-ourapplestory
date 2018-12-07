//
// Created by kyle on 20/10/18.
//

#ifndef VCALCBASE_INTERNALTOOLS_H
#define VCALCBASE_INTERNALTOOLS_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "AST/ASTNodes/BaseNodes/ASTNode.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/IR/Verifier.h"
#include "CondBuilder.h"
#include "WhileBuilder.h"
#include "globals.h"
#include "../Scope/GazpreaTupleType.h"

class InternalTools {
public:
    struct pair{
        pair():left(nullptr),right(nullptr){};
        llvm::Value *left;
        llvm::Value *right;
    };

    struct tupleGarbo {
    	tupleGarbo():type(nullptr), leftIndex(-1), rightIndex(-1){};
		llvm::Type *type;
    	int leftIndex;
    	int rightIndex;
    };

    InternalTools(llvm::LLVMContext *globalCtx, llvm::IRBuilder<> *ir, llvm::Module *mod);
    void setUpTypes();
    //tools
	llvm::Value *getValFromTuple(llvm::Value *tuplePtr, llvm::Value *idx);
	llvm::Value *getPtrFromTuple(llvm::Value *tuplePtr, llvm::Value *idx);
	llvm::Value *getValFromStruct(llvm::Value * sPtr, llvm::Value *idx);
	llvm::Value *getPtrFromStruct(llvm::Value * sPtr, llvm::Value *idx);
	llvm::Value *getValFromStruct(llvm::Value * sPtr, int idx);
	llvm::Value *getPtrFromStruct(llvm::Value * sPtr, int idx);
	//llvm::Value *initTuple(llvm::Value *tuplePtr, std::vector<llvm::Value *> *values);
	llvm::Value *initTupleFromPtrs(llvm::Value *tuplePtr, std::vector<llvm::Value *> *ptrs);
	std::vector<llvm::Value *> *getPtrVectorFromStruct(llvm::Value *structPtr);
	llvm::Value *geti1(int64_t val);
    llvm::Value *geti8(int64_t val);
    llvm::Value *getReal(float val);
    llvm::Value *getConsi32(int64_t val);
    llvm::Value *getConsi64(int64_t val);
    llvm::Value *safeReplace(llvm::Value *safePtr, llvm::Value *cond, llvm::Value *idx,
                             llvm::Value *arrP, llvm::Value *replaceVal);
    llvm::Value *getNull(llvm::Type *type);
    llvm::Value *getIdn(llvm::Type *type);
    llvm::Value *getAdd(llvm::Value *left, llvm::Value * right);
    llvm::Value *getSub(llvm::Value *left, llvm::Value * right);
    llvm::Value *getMul(llvm::Value *left, llvm::Value * right);
    llvm::Value *getDiv(llvm::Value *left, llvm::Value * right);
	llvm::Value *getRem(llvm::Value *left, llvm::Value * right);
    llvm::Value *getEQ (llvm::Value *left, llvm::Value * right);
    llvm::Value *getNEQ(llvm::Value *left, llvm::Value * right);
    llvm::Value *getGT (llvm::Value *left, llvm::Value * right);
    llvm::Value *getLT (llvm::Value *left, llvm::Value * right);
    llvm::Value *getLTE(llvm::Value *left, llvm::Value * right);
    llvm::Value *getGTE(llvm::Value *left, llvm::Value * right);
    llvm::Value *getAnd(llvm::Value *left, llvm::Value * right);
    llvm::Value *getOr (llvm::Value *left, llvm::Value * right);
    llvm::Value *getXOr(llvm::Value *left, llvm::Value * right);
    llvm::Value *getNegation(llvm::Value *expr);
    llvm::Value *getUnarySub(llvm::Value *expr);

    llvm::Value *castMatrixIndex(llvm::Value *slice, llvm::Value *l, llvm::Value *r, llvm::Value *mat);
    llvm::Value *getMatrixNumRows(llvm::Value * mat);
	llvm::Value *getMatrixNumCols(llvm::Value * mat);

    pair makePair(llvm::Value *left, llvm::Value *right);
    tupleGarbo makeGarbo(llvm::Type *type, int leftIndex = -1, int rightIndex = -1);

	InternalTools::tupleGarbo parseStringExtension(const std::string &typeString);
	std::vector<std::string> split(const std::string& s, char delimiter);

	bool setIdentity(llvm::Type * type, llvm::Value * ptr);
	bool setNull(llvm::Type * type, llvm::Value * ptr);
    bool isStructType(llvm::Value *ptr);
	bool isTupleType(llvm::Value *ptr);
    bool isVectorType(llvm::Value *ptr);
	bool isMatrixType(llvm::Value *ptr);
    bool isDeclVectorType(llvm::Type *type);
    bool isDeclMatrixType(llvm::Type *type);
    bool isIntervalType(llvm::Value *ptr);
    std::vector<llvm::Value *> * getValueVectorFromTuple(llvm::Value *structPtr);
    std::string getType(llvm::Type * type, llvm::Value *expr);
    llvm::Value *getConstFromType(llvm::Type* ty);
    llvm::Type *getVectorElementType(llvm::Value *vec);
	llvm::Type *getMatrixVectorType(llvm::Value *mat);
	llvm::Type *getMatrixElementType(llvm::Value *mat);
    llvm::Value *castVectorToType(llvm::Value *vec, llvm::Type *type);
	llvm::Value *castMatrixToType(llvm::Value *mat, llvm::Type *type);
    llvm::Value *setVectorValues(llvm::Value *vec, std::vector<llvm::Value *> *values);
	llvm::Type *getVectorType(const std::string &typeString);
	llvm::Type *getInitVectorType(std::vector<llvm::Type *> &types);
	llvm::Type *getDeclScalarTypeFromVec(llvm::Type *type);
	llvm::Type *getDeclScalarTypeFromMat(llvm::Type *type);
	llvm::Type *getDeclMatrixType(const std::string &typeString);
	llvm::Type *getDeclVectorType(const std::string &typeString);
	std::string getVectorTypeString(llvm::Value *vec);
	int getVectorLength(llvm::Value *vec);

protected:
    llvm::LLVMContext * globalCtx;
    llvm::IRBuilder<> * ir;
    llvm::Module      * mod;
};

#endif //VCALCBASE_INTERNALTOOLS_H
