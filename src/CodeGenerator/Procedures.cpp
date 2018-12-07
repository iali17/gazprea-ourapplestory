//
// Created by iali1 on 11/4/18.
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
extern llvm::Type *intervalTy;

/**
 * Handles calls to procedures and functions
 *
 * @param node
 * @return llvm::Value *
 */
llvm::Value *CodeGenerator::visit(CallNode *node) {
    return callFuncOrProc(node->getProcedureName(), node->getExprNodes());
}

/**
 * Handles procedures
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(ProcedureNode *node) {
    //create function
    llvm::Function *F = declareFuncOrProc(node->getProcedureName(), node->getRetType(), node->getParamNodes(),
                                          node->getType(), node->getLine(), node->getTupleType(), node->getGType());

    //generate body
    generateFuncOrProcBody(F, node->getParamNodes(), node->getBlock());

    if(!F->getBasicBlockList().back().getTerminator()){
        if(F->getReturnType() == llvm::Type::getVoidTy(*globalCtx)){
            ir->CreateRetVoid();
        } else if (F->getBasicBlockList().back().empty()) {
            F->getBasicBlockList().back().eraseFromParent();
        } else {
            std::cerr << "Missing return statement in function defined at line " << node->getLine() << ". Aborting...\n";
            exit(1);
        }
    }

    return nullptr;
}

/**
 * Handles functions
 *
 * @param node
 * @return nullptr
 */
llvm::Value *CodeGenerator::visit(FunctionNode *node) {
    //create function
    llvm::Function *F = declareFuncOrProc(node->getFunctionName(), node->getRetType(), node->getParamNodes(),
            node->getType(), node->getLine(), node->getTupleType(), node->getGType());

    //generate body
    generateFuncOrProcBody(F, node->getParamNodes(), node->getBlock());

    /* //TODO THIS DOES NOT ENFORCE FLOW BUT YEaH
    if(!F->getBasicBlockList().back().getTerminator()){
        std::cerr << "Missing return statement in function defined at line: " << node->getLine() << '\n';
        exit(13);
    }*/
    if (F->getBasicBlockList().back().empty()) {
        F->getBasicBlockList().back().eraseFromParent();
    }

    return nullptr;
}

llvm::Value *CodeGenerator::visit(ProtoProcedureNode * node) {
    declareFuncOrProc(node->getProcedureName(), node->getRetType(), node->getParamNodes(),
            node->getType(), node->getLine(), node->getTupleType(), node->getGType());
    return nullptr;
}
