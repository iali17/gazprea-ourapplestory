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

std::vector<llvm::Value *> CodeGenerator::getParamVec(std::vector<ASTNode *> *paramNode,std::vector<ASTNode *> *arguNode ) {
    std::vector<llvm::Value *> paramVector;

    // TODO: All the other types :(
    for (unsigned int i = 0; i < arguNode->size(); ++i) {
        if ((arguNode->at(i)->getType()) == CHAR) {
            llvm::Value* ptr = ir->CreateAlloca(charTy);
            llvm::Value* val = visit(arguNode->at(i));
            ir->CreateStore(val, ptr);
            paramVector.push_back(ptr);
        } else if ((arguNode->at(i)->getType()) == INTEGER) {
            llvm::Value* ptr = ir->CreateAlloca(intTy);
            llvm::Value* val = visit(arguNode->at(i));
            ir->CreateStore(val, ptr);
            paramVector.push_back(ptr);
        }  else if ((arguNode->at(i)->getType()) == REAL) {
            llvm::Value *ptr = ir->CreateAlloca(realTy);
            llvm::Value *val = visit(arguNode->at(i));
            ir->CreateStore(val, ptr);
            paramVector.push_back(ptr);
        } else {
            // We are passing in a variable
            llvm::Value *p;
            auto *pNode = dynamic_cast<ParamNode *>(paramNode->at(i));
            bool constant = pNode->isIsVar();
            Symbol * idNode = symbolTable->resolveSymbol(((IDNode *) arguNode->at(i))->getID());

            if (!constant) {
                if (!idNode->isConstant()){
                    p = idNode->getPtr();
                } else {
                    llvm::Value *ptr = ir->CreateAlloca(idNode->getPtr()->getType()->getPointerElementType());
                    llvm::Value *val = ir->CreateLoad(idNode->getPtr());
                    ir->CreateStore(val, ptr);
                    p = ptr;
                }
            } else {
                llvm::Value *ptr = ir->CreateAlloca(idNode->getPtr()->getType()->getPointerElementType());
                llvm::Value *val = ir->CreateLoad(idNode->getPtr());
                ir->CreateStore(val, ptr);
                p = ptr;
            }

            paramVector.push_back(p);
        }
    }

    return paramVector;
}

llvm::StructType *CodeGenerator::parseStructType(TupleType *node) {
    auto *declNodes  = node->getDecls();
    auto *members    = new std::vector<llvm::Type *>;
    llvm::Type* type;
    auto *memberNames = new std::unordered_map<std::string, int>;
    int i = 0;
    for (auto element : * declNodes) {
        type = symbolTable->resolveType(((DeclNode *) element)->getTypeIds()->at(0))->getTypeDef();
        members->push_back(type->getPointerTo());
        if(!(((DeclNode *) element)->getID().empty()))
            memberNames->insert(std::pair<std::string, int> (((DeclNode *) element)->getID(), i));
        i++;
    }

    llvm::StructType * newStruct = llvm::StructType::create(*members, "tuple");
    symbolTable->addTupleType(newStruct, memberNames, members); //this is where we add the struct to the symbol table
    //symbolTable->addTupleType("ugh", newStruct, memberNames, members);
    return newStruct;
}

llvm::Value *CodeGenerator::getIndexForTuple(ASTNode *index, llvm::Value *tuplePtr) {
    llvm::Value *idx;
    if(dynamic_cast<IDNode *>(index)){
        llvm::Type * type = tuplePtr->getType()->getPointerElementType();
        assert(type->isStructTy());
        auto *structType = llvm::cast<llvm::StructType>(type);
        std::string s = structType->getStructName();
        //llvm::Type *t = type->getPointerTo();
        GazpreaTupleType *gtt = symbolTable->resolveTupleType(structType);
        int i = gtt->getOffsetFromString(dynamic_cast<IDNode *>(index)->getID());
        idx = it->getConsi32(i);
    }
    else{
        idx = visit(index);
    }

    return idx;
}

