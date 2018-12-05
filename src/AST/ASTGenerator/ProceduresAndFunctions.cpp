//
// Created by iali on 11/24/18.
//

#include <AST/ASTGenerator.h>

antlrcpp::Any ASTGenerator::visitProcedureCall(gazprea::GazpreaParser::ProcedureCallContext *ctx) {
    std::string procName = ctx->Identifier()->getText();
    if (procName != "main") procName = normalizeID(procName);
    if(inFunction) {
        auto ret = functionNames->find(procName);
        assert(ret != functionNames->end());

    } else {
        auto ret = procedureNames->find(procName);
        assert(ret != procedureNames->end());
    }

    auto *exprNodes = new std::vector<ASTNode*>;
    for(unsigned long i = 0; i < ctx->expr().size(); ++i) {
        ASTNode * node = (ASTNode *) visit(ctx->expr()[i]);
        exprNodes->push_back(node);
    }
    return (ASTNode *) new CallNode(exprNodes, procName, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitProcedure(gazprea::GazpreaParser::ProcedureContext *ctx) {
    std::string procName = ctx->Identifier()->getText();
    if (procName != "main") procName = normalizeID(procName);
    auto ret = procedureNames->find(procName);
    if(ret == procedureNames->end())
        procedureNames->insert(procName);
    std::string retType = "void";
    TupleTypeNode * tupleType = nullptr;
    int gType = -1;


    if(ctx->returnStat() && ctx->returnStat()->type()->tupleType()){
        tupleType = (TupleTypeNode *) (ASTNode *) visit(ctx->returnStat()->type()->tupleType());
    } else if (ctx->returnStat() && ctx->returnStat()->type()->vectorType()){
        gType = VECTOR_T;
    } else if(ctx->returnStat() && ctx->returnStat()->type()->matrixType()){
        gType = MATRIX;
    }
    if(ctx->returnStat()){
        retType = ctx->returnStat()->type()->getText();
    }

    BlockNode *block;

    //block
    if(ctx->oneLineFunction())
        block = (BlockNode *) (ASTNode *) visit(ctx->oneLineFunction());
    else
        block  = (BlockNode *) (ASTNode *) visit(ctx->block());

    std::vector<ASTNode *> *params = (std::vector<ASTNode *> *) visit(ctx->params());
    ASTNode * p = (ASTNode *) new ProcedureNode(block, params, retType, procName, (int)ctx->getStart()->getLine(), tupleType, gType);
    return p;
}

/**
 * TODO - not this
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitParams(gazprea::GazpreaParser::ParamsContext *ctx) {
    //returns a vector or parameter nodes
    auto *paramVec = new std::vector<ASTNode *>;
    int gType;
    std::string declaredType;
    bool constant;
    for (unsigned long i = 0; i < ctx->param().size(); ++i){
        constant = nullptr == ctx->param().at(i)->VAR();
        gType = -1;
        declaredType = ctx->param().at(i)->type()->getText();

        if(ctx->param().at(i)->type()->vectorType()) gType = VECTOR_T;
        else if (ctx->param().at(i)->type()->matrixType()) gType = MATRIX;

        if(ctx->param().at(i)->extension() && !ctx->param().at(i)->extension()->rightExtension()) {
            if(!ctx->param().at(i)->type()->vectorType()){
                declaredType += "vector";
            }
            gType = VECTOR_T;
            declaredType +=  ctx->param().at(i)->extension()->getText();
        }
        else if(ctx->param().at(i)->extension() && ctx->param().at(i)->extension()->rightExtension()) {
            if(!ctx->param().at(i)->type()->matrixType()){
                declaredType += "matrix";
            }
            gType = MATRIX;
            declaredType +=  ctx->param().at(i)->extension()->getText();
        }


        if (ctx->param().at(i)->type()->tupleType()) {
            TupleTypeNode *tt = (TupleTypeNode *) ((ASTNode *) visit(ctx->param().at(i)->type()->tupleType()));
            paramVec->push_back(new ParamNode(ctx->param().at(i)->type()->getText(),
                                              normalizeID(ctx->param().at(i)->Identifier()->getText()), constant,
                                              (int)ctx->getStart()->getLine(), tt));
        } else {
            paramVec->push_back(new ParamNode(declaredType,
                                              normalizeID(ctx->param().at(i)->Identifier()->getText()), constant, (int)ctx->getStart()->getLine(),
                                              nullptr, gType));
        }

    }

    return paramVec;
}

antlrcpp::Any ASTGenerator::visitReturnStat(gazprea::GazpreaParser::ReturnStatContext *ctx) {
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitReturnCall(gazprea::GazpreaParser::ReturnCallContext *ctx) {
    ASTNode * expr = nullptr;
    if(ctx->expr())
        expr = (ASTNode *) visit(ctx->expr());
    return (ASTNode *) new ReturnNode(expr, (int)ctx->getStart()->getLine());
}

/**
 * same as procedure but with required type
 *
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitFunction(gazprea::GazpreaParser::FunctionContext *ctx) {
    inFunction = true;
    TupleTypeNode * tupleType = nullptr;
    int gType = -1;
    std::string retType = ctx->returnStat()->type()->getText();

    if(ctx->returnStat()->type()->tupleType()){
        tupleType = (TupleTypeNode *) (ASTNode *) visit(ctx->returnStat()->type()->tupleType());
    } else if (ctx->returnStat() && ctx->returnStat()->type()->vectorType()) {
        gType = VECTOR_T;
    }else if(ctx->returnStat() && ctx->returnStat()->type()->matrixType()){
        gType = MATRIX;
    }
    auto ret = functionNames->find(normalizeID(ctx->Identifier()->getText()));
    if(ret == functionNames->end())
        functionNames->insert(normalizeID(ctx->Identifier()->getText()));
    BlockNode *block  = (BlockNode *) (ASTNode *) visit(ctx->functionReturns());
    std::vector<ASTNode *> *params = (std::vector<ASTNode *> *) visit(ctx->params());
    ASTNode * f = (ASTNode *) new FunctionNode(block, params, retType, normalizeID(ctx->Identifier()->getText()),
                                               (int)ctx->getStart()->getLine(), tupleType, gType);

    inFunction = false;
    return f;
}

/**
 * i will return a block always. if there is a '=' i will replace with an return statement in a block
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitFunctionReturns(gazprea::GazpreaParser::FunctionReturnsContext *ctx) {
    ASTNode *blockNode;
    if(ctx->oneLineFunction()){
        blockNode = (ASTNode *) visit(ctx->oneLineFunction());
    }
    else {
        if(ctx->block()->single_statement() && !ctx->block()->single_statement()->returnCall()){
            std::cerr << "Missing return call on line " << ctx->getStart()->getLine() << "\n";
        }
        blockNode = (ASTNode *) visit(ctx->block());
    }
    return blockNode;
}

antlrcpp::Any ASTGenerator::visitProcProto(gazprea::GazpreaParser::ProcProtoContext *ctx) {
    std::string procName = ctx->Identifier()->getText();
    int gType = -1;
    if (procName != "main") procName = normalizeID(procName);
    auto ret = procedureNames->find(procName);
    if(ret == procedureNames->end())
        procedureNames->insert(procName);
    std::string retType = "void";
    TupleTypeNode * tupleType = nullptr;
    if(ctx->returnStat() && ctx->returnStat()->type()->tupleType()){
        tupleType = (TupleTypeNode *) (ASTNode *) visit(ctx->returnStat()->type()->tupleType());
    }else if (ctx->returnStat() &&ctx->returnStat()->type()->vectorType()) {
        gType = VECTOR_T;
    }else if(ctx->returnStat() && ctx->returnStat()->type()->matrixType()){
        gType = MATRIX;
    }
    if(ctx->returnStat()) retType = ctx->returnStat()->type()->getText();
    std::vector<ASTNode *> *params = (std::vector<ASTNode *> *) visit(ctx->params());
    ASTNode * p = (ASTNode *) new ProtoProcedureNode(params, retType, procName,
                                                     (int)ctx->getStart()->getLine(), tupleType, gType);
    return p;
}

antlrcpp::Any ASTGenerator::visitFuncProto(gazprea::GazpreaParser::FuncProtoContext *ctx) {
    auto ret = functionNames->find(normalizeID(ctx->Identifier()->getText()));
    int gType = -1;
    if(ret == functionNames->end())
        functionNames->insert(normalizeID(ctx->Identifier()->getText()));
    std::string retType = ctx->returnStat()->type()->getText();
    TupleTypeNode * tupleType = nullptr;
    if(ctx->returnStat()->type()->tupleType()){
        tupleType = (TupleTypeNode *) (ASTNode *) visit(ctx->returnStat()->type()->tupleType());
    }else if (ctx->returnStat() && ctx->returnStat()->type()->vectorType()) {
        gType = VECTOR_T;
    }else if(ctx->returnStat() && ctx->returnStat()->type()->matrixType()){
        gType = MATRIX;
    }
    std::vector<ASTNode *> *params = (std::vector<ASTNode *> *) visit(ctx->params());
    ASTNode * p = (ASTNode *) new ProtoProcedureNode(params, retType, normalizeID(ctx->Identifier()->getText()),
                                                     (int)ctx->getStart()->getLine(), tupleType, gType);
    return p;
}

antlrcpp::Any ASTGenerator::visitFunctionCall(gazprea::GazpreaParser::FunctionCallContext *ctx) {
    std::string functionName = normalizeID(ctx->Identifier()->getText());

    // Checks if in a function
    if(inFunction) {
        auto ret = functionNames->find(normalizeID(ctx->Identifier()->getText()));
        assert(ret != functionNames->end());
    }

    auto *exprNodes = new std::vector<ASTNode*>;
    for(unsigned long i = 0; i < ctx->expr().size(); ++i) {
        ASTNode * node = (ASTNode *) visit(ctx->expr()[i]);
        exprNodes->push_back(node);
    }
    return (ASTNode *) new CallNode(exprNodes, functionName, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitOneLineFunction(gazprea::GazpreaParser::OneLineFunctionContext *ctx) {
    ASTNode *blockNode;
    ASTNode * expr    = (ASTNode *) visit(ctx->expr());
    ASTNode * retNode = new ReturnNode(expr,  (int)ctx->getStart()->getLine());
    auto *declBlock = new std::vector<ASTNode *>();
    auto *statBlock = new std::vector<ASTNode *>();
    auto *declBlockNode = new BasicBlockNode(declBlock, (int)ctx->getStart()->getLine());
    auto *statBlockNode = new BasicBlockNode(statBlock, (int)ctx->getStart()->getLine());
    statBlock->push_back(retNode);
    return (ASTNode *) new BlockNode(declBlockNode, statBlockNode, (int)ctx->getStart()->getLine());
}

