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

// MAKE THIS LESS GARBAGE but it works soo do it later
std::vector<llvm::Value *> CodeGenerator::getParamVec(std::vector<ASTNode *> *paramNode,std::vector<ASTNode *> *arguNode ) {
    std::vector<llvm::Value *> paramVector;
    std::vector<std::string> aliasVector;

    int arguNodeType;
    llvm::Value* ptr;
    llvm::Value* val;
    ParamNode *pNode;
    Symbol * idNode;
    bool constant;
    llvm::Value *idx;

    for (unsigned int i = 0; i < arguNode->size(); ++i) {
        pNode = dynamic_cast<ParamNode *>(paramNode->at(i));
        arguNodeType = arguNode->at(i)->getType();
        constant = pNode->isIsVar();

        if((arguNodeType == CHAR) || (arguNodeType == INTEGER) || (arguNodeType == REAL)) {
            ptr = ir->CreateAlloca(arguNode->at(i)->getLlvmType());
            val = visit(arguNode->at(i));
            ir->CreateStore(val, ptr);
        } else if (dynamic_cast<IndexTupleNode *>(arguNode->at(i))) {
            auto tupNode = dynamic_cast<IndexTupleNode *>(arguNode->at(i));
            auto idxNode = dynamic_cast<INTNode *>(tupNode->getIndex());
            idNode = symbolTable->resolveSymbol(tupNode->getIdNode()->getID());
            idx = getIndexForTuple(tupNode->getIndex(), idNode->getPtr());
            ptr = getPtrToVar(idNode, constant, aliasVector, idx, idxNode->value);

        } else {
            idNode = symbolTable->resolveSymbol(((IDNode *) arguNode->at(i))->getID());
            ptr = getPtrToVar(idNode, constant, aliasVector);
        }
        paramVector.push_back(ptr);
    }

    return paramVector;
}

llvm::Value *CodeGenerator::getPtrToVar(Symbol *idNode, bool constant, std::vector<std::string> &aliasVector,
        llvm::Value *idxVal, int idxTrueVal) {
    llvm::Value *ptr, *val;
    std::string name;

    if (!constant) {
        assert(!idNode->isConstant());
        if (idxVal){
            ptr = it->getPtrFromTuple(idNode->getPtr(),idxVal);
            name = idNode->getName() + std::to_string(idxTrueVal);
        } else {
            ptr = idNode->getPtr();
            name = idNode->getName();
        }

        if (std::find(aliasVector.begin(), aliasVector.end(), "c"+name) != aliasVector.end()
            || std::find(aliasVector.begin(), aliasVector.end(), name) != aliasVector.end()){
            std::cerr << "Aliasing error\nAborting...\n";
            exit(1);
        }
        aliasVector.push_back(idNode->getName());
        return ptr;
    } else {
        if (idxVal) {
            val = it->getValFromTuple(idNode->getPtr(),idxVal);
            ptr = ir->CreateAlloca(val->getType());
            name = idNode->getName() + std::to_string(idxTrueVal);
        } else {
            ptr = ir->CreateAlloca(idNode->getPtr()->getType()->getPointerElementType());
            val = ir->CreateLoad(idNode->getPtr());
            name = idNode->getName();
        }
        ir->CreateStore(val, ptr);

        if (std::find(aliasVector.begin(), aliasVector.end(), name) != aliasVector.end()){
            std::cerr << "Aliasing error\nAborting...\n";
            exit(1);
        }
        aliasVector.push_back("c" + idNode->getName());
        return ptr;
    }
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
    return newStruct;
}

llvm::Value *CodeGenerator::getIndexForTuple(ASTNode *index, llvm::Value *tuplePtr) {
    llvm::Value *idx;
    if(dynamic_cast<IDNode *>(index)){
        llvm::Type * type = tuplePtr->getType()->getPointerElementType();
        assert(type->isStructTy());
        auto *structType = llvm::cast<llvm::StructType>(type);
        GazpreaTupleType *gtt = symbolTable->resolveTupleType(structType);
        int i = gtt->getOffsetFromString(dynamic_cast<IDNode *>(index)->getID());
        idx = it->getConsi32(i);
    }
    else{
        idx = visit(index);
    }

    return idx;
}

