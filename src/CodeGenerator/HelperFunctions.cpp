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

std::vector<llvm::Value *> CodeGenerator::getParamVec(std::vector<ParamNode *> *exprNode) {
    std::vector<llvm::Value *> dumb;

    // TODO: All the other types :(
    for (unsigned int i = 0; i < exprNode->size(); ++i) {
        if ((exprNode->at(i)->getType()) == CHAR) {
            llvm::Value* ptr = ir->CreateAlloca(charTy);
            llvm::Value* val = visit(exprNode->at(i));
            ir->CreateStore(val, ptr);
            dumb.push_back(ptr);
        } else if ((exprNode->at(i)->getType()) == INTEGER) {
            llvm::Value* ptr = ir->CreateAlloca(intTy);
            llvm::Value* val = visit(exprNode->at(i));
            ir->CreateStore(val, ptr);
            dumb.push_back(ptr);
        }  else if ((exprNode->at(i)->getType()) == REAL) {
            llvm::Value *ptr = ir->CreateAlloca(realTy);
            llvm::Value *val = visit(exprNode->at(i));
            ir->CreateStore(val, ptr);
            dumb.push_back(ptr);
        } else {
            // We are passing in a variable
            llvm::Value *dumb2;
            ParamNode *yes = dynamic_cast<ParamNode *>(exprNode->at(i));
            bool constant = yes->isIsVar();
            Symbol * wtf = symbolTable->resolveSymbol(((IDNode *) exprNode->at(i))->getID());

            if (!constant) {
                if (!wtf->isConstant()){
                    dumb2 = wtf->getPtr();
                } else {
                    llvm::Value *ptr = ir->CreateAlloca(wtf->getPtr()->getType()->getPointerElementType());
                    llvm::Value *val = ir->CreateLoad(wtf->getPtr());
                    ir->CreateStore(val, ptr);
                    dumb2 = ptr;
                }
            } else {
                llvm::Value *ptr = ir->CreateAlloca(wtf->getPtr()->getType()->getPointerElementType());
                llvm::Value *val = ir->CreateLoad(wtf->getPtr());
                ir->CreateStore(val, ptr);
                dumb2 = ptr;
            }

            dumb.push_back(dumb2);
        }
    }

    return dumb;
}