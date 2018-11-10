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

llvm::Value *CodeGenerator::visit(CallNode *node) {
    return callFuncOrProc(node->getProcedureName(), node->getExprNodes());
}

llvm::Value *CodeGenerator::visit(FunctionCallNode *node) {
    return callFuncOrProc(node->getFunctionName(), node->getArguments());
}

llvm::Value *CodeGenerator::visit(ProcedureNode *node) {
    //create function
    llvm::Function *F = declareFuncOrProc(node->getProcedureName(), node->getRetType(), node->getParamNodes(),
                                          node->getType(), node->getLine(), node->getTupleType());

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

llvm::Value *CodeGenerator::visit(FunctionNode *node) {
    //create function
    llvm::Function *F = declareFuncOrProc(node->getFunctionName(), node->getRetType(), node->getParamNodes(),
            node->getType(), node->getLine(), node->getTupleType());

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

llvm::Value* CodeGenerator::visit(ProcedureCallNode *node) {
    llvm::Value *val = callFuncOrProc(node->getProcedureName(), node->getExprNode());

    if (node->getUnOp() == NEG) {
        if (val->getType() == realTy){
            val  =  ir->CreateFNeg(val, "fnegtmp");
        } else {
            val = ir->CreateNeg(val, "negtmp");
        }
    } else if (node->getUnOp() == MINUS) {
        if(val->getType() == intTy){
            val =  ir->CreateSub(it->getConsi32(0), val, "iaddtmp");
        }
        else if(val->getType() == realTy){
            val =  ir->CreateFSub(it->getConsi32(0), val, "faddtmp");
        }
    }

    llvm::Value* ptr = nullptr;

    if (node->getTypeIds()->size() == 0){
        if(it->isStructType(val)){
            ptr = val;
            symbolTable->addSymbol(node->getVarName(), node->getType(), true);

        } else {
            ptr = ir->CreateAlloca(val->getType());
            ir->CreateStore(val, ptr);
            symbolTable->addSymbol(node->getVarName(), node->getType(), true);
        }

    } else if (node->getTypeIds()->size() == 1){
        if(it->isStructType(val)){
            //ptr = val;
            //Symbol * symbol = symbolTable->resolveSymbol(node->getVarName());
            llvm::StructType *structType = parseStructType(node->getTupleType());
            ptr = ir->CreateAlloca(structType);
            ptr = it->initTuple(ptr, it->getValueVectorFromStruct(val));
            symbolTable->addSymbol(node->getVarName(), node->getType(), true);

        } else {
            llvm::Type *type = symbolTable->resolveType(node->getTypeIds()->at(0))->getTypeDef();
            ptr = ir->CreateAlloca(type);
            if (val == nullptr) {
                if (!(it->setNull(type, ptr))) {
                      std::cerr << "Unable to initialize to null at line " << node->getLine() << ". Aborting...\n";
                      exit(1);
                }
                symbolTable->addSymbol(node->getVarName(), node->getType(), node->isConstant(), ptr);
                return nullptr;
            }
            ir->CreateStore(val, ptr);
            symbolTable->addSymbol(node->getVarName(), node->getType(), node->isConstant());
        }
    }

    symbolTable->resolveSymbol(node->getVarName())->setPtr(ptr);

    return nullptr;
}

llvm::Value *CodeGenerator::visit(ParamNode *node) {
    return nullptr;
}

llvm::Value *CodeGenerator::visit(ProtoProcedureNode * node) {
    declareFuncOrProc(node->getProcedureName(), node->getRetType(), node->getParamNodes(),
            node->getType(), node->getLine(), node->getTupleType());
    return nullptr;
}
