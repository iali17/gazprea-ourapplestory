//
// Created by kyle on 20/10/18.
//

#include <CodeGenerator/CodeGenerator.h>


extern llvm::Type *i64Ty;
extern llvm::Type *i32Ty;
extern llvm::Type *intTy;
extern llvm::Type *i8Ty;
extern llvm::Type *charTy;

llvm::Value *CodeGenerator::visit(FileNode *node) {
    // register external functions
    et->registerPrintf();
    et->registerFree();
    et->registerCalloc();

    unsigned long i = 0;
    for(i = 0; i < node->nodes->size(); i++){
        ASTBaseVisitor::visit(node->nodes->at(i));
    }
    return nullptr;
}

llvm::Value *CodeGenerator::visit(BasicBlockNode *node) {
    unsigned long i = 0;
    for(i = 0; i < node->nodes->size(); i++){
        ASTBaseVisitor::visit(node->nodes->at(i));
    }
    return nullptr;
}

llvm::Value *CodeGenerator::visit(ASTNode *node) {
    return ASTBaseVisitor::visit(node);
}

CodeGenerator::CodeGenerator(char *outFile) : outFile(outFile) {
}

llvm::Value *CodeGenerator::visit(ProcedureNode *node) {
    //TODO - actual fcn type and params
    llvm::FunctionType *funcTy = llvm::TypeBuilder<int(), false>::get(*globalCtx);

    auto *func = llvm::cast<llvm::Function>(mod->getOrInsertFunction(node->getProcedureName(), funcTy));

    // Create an entry block and set the inserter.
    llvm::BasicBlock *entry = llvm::BasicBlock::Create(*globalCtx, "entry", func);
    ir->SetInsertPoint(entry);

    visit(node->getFullBlock());

    return nullptr;
}

llvm::Value *CodeGenerator::visit(ParamNode *node) {
    return ASTBaseVisitor::visit(node);
}

llvm::Value *CodeGenerator::visit(ReturnNode *node) {

    llvm::Value * ret = visit(node->getExpr());
    ir->CreateRet(ret);
    return nullptr;
}

llvm::Value *CodeGenerator::visit(INTNode *node) {
    return it->getConsi32(node->value);
}
