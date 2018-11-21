//
// Created by ali5 on 11/21/18.
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
 * Deals with implicit and explicit casting.
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(CastExprNode *node) {
    llvm::Type *type;
    llvm::Value *expr;

    if(node->getTuple() || it->isStructType(visit(node->getExpr()))) {
        return tupleCasting(node);
    } else if(node->getTypeString() == "integervector") {
        return vectorCasting(node);
    } else {
        expr = visit(node->getExpr());
        GazpreaType *gazpreaType = symbolTable->resolveType(node->getTypeString());
        type = gazpreaType->getTypeDef();

        return ct->varCast(type, expr, node->getLine());
    }
}

/**
 * Deals with tuple casting
 *
 * @param node
 * @return
 */
llvm::Value *CodeGenerator::tupleCasting(CastExprNode *node) {
    llvm::Type *type;
    llvm::Value *expr;

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
}

/**
 * Deals with vector casting
 *
 * @param node
 * @return
 */
llvm::Value* CodeGenerator::vectorCasting(CastExprNode *node) {

}

/**
 * Deals with matrix casting
 *
 * @param node
 * @return
 */
llvm::Value* CodeGenerator::matrixCasting(CastExprNode *node) {

}