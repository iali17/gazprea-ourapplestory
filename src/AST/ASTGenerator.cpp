//
// Created by kyle on 20/10/18.
//
#include <AST/ASTGenerator.h>

/**
 * visits the file
 *  - typedefs, globals, forward decl, procedures and functions
 *
 * @param ctx
 * @return  - root of tree
 */
antlrcpp::Any ASTGenerator::visitFile(gazprea::GazpreaParser::FileContext *ctx) {
    auto *procedures = new std::vector<ASTNode *>;
    for (unsigned long i = 0; i < ctx->termsAndConditions().size(); ++i){
        ASTNode * p = (ASTNode *) visit(ctx->termsAndConditions().at(i));
        if (nullptr == p){
            printf("yes\n");
        }
        procedures->push_back(p);
    }
    return (ASTNode *) new FileNode(procedures, (int)ctx->getStart()->getLine());
}

/**
 * Integer Expr
 *  - literal integer expression
 * @param ctx
 * @return - INTNode
 */
antlrcpp::Any ASTGenerator::visitIntegerExpr(gazprea::GazpreaParser::IntegerExprContext *ctx) {
    std::string rawStr = ctx->getText();
    rawStr.erase(std::remove(rawStr.begin(), rawStr.end(), '_'), rawStr.end());     // remove the underscores
    int val = std::stoi(rawStr);
    return (ASTNode *) new INTNode(val, (int) ctx->getStart()->getLine());
}

/**
 * Null Identifier
 * @param ctx
 * @return - NullNode
 */
antlrcpp::Any ASTGenerator::visitNullExpr(gazprea::GazpreaParser::NullExprContext *ctx) {
    return (ASTNode *) new NullNode((int)ctx->getStart()->getLine());
}

/**
 * Explicit Cast
 *  - for casts requested by user
 * @param ctx
 * @return - CastExprNode
 */
antlrcpp::Any ASTGenerator::visitCastExpr(gazprea::GazpreaParser::CastExprContext *ctx) {
    ASTNode * expr = (ASTNode *) visit(ctx->expr());

    if(ctx->type()->tupleType()) {
        ASTNode *tuple = (ASTNode *) visit(ctx->type()->tupleType());

        return (ASTNode *) new TupleCastNode(expr, tuple, (int)ctx->getStart()->getLine());
    } else if(ctx->type()->vectorType()) {
        ASTNode *vector = (ASTNode *) visit(ctx->type()->vectorType());

        return (ASTNode *) new VectorCastNode(expr, vector, (int)ctx->getStart()->getLine());
    } else if(ctx->type()->matrixType()) {
        ASTNode *matrix = (ASTNode *) visit(ctx->type()->matrixType());

        return (ASTNode *) new MatrixCastNode(expr, matrix, (int)ctx->getStart()->getLine());
    }

    return (ASTNode *) new ScalarCastNode(expr, ctx->type()->getText(), (int)ctx->getStart()->getLine());
}

/**
 * Real Literal
 *  Eg: 2.0
 * @param ctx
 * @return - RealNode
 */
antlrcpp::Any ASTGenerator::visitRealExpr(gazprea::GazpreaParser::RealExprContext *ctx) {
    std::string strVal = ctx->getText();
    std::string str2Val;

    std::copy_if (strVal.begin(), strVal.end(), std::back_inserter(str2Val), [](char i){return i != '_';} ); // remove _
    float val = std::stof(str2Val);
    return (ASTNode *) new RealNode(val, (int)ctx->getStart()->getLine());
}

/**
 * Expr in Brackets
 * @param ctx
 * @return - Node of expression in bracket
 */
antlrcpp::Any ASTGenerator::visitBrackExpr(gazprea::GazpreaParser::BrackExprContext *ctx) {
    return (ASTNode *) visit(ctx->expr());
}

antlrcpp::Any ASTGenerator::visitIntervalExpr(gazprea::GazpreaParser::IntervalExprContext *ctx) {
    ASTNode *leftExpr;
    ASTNode *rightExpr = (ASTNode *) visit(ctx->right);

    if(ctx->IntervalThing()) {
        std::string IntervalText = ctx->IntervalThing()->getText();
        std::vector<std::string> values = split(IntervalText,'.');
        std::string idName = values[0];

        leftExpr = getIndexNode(values, 0, (int)ctx->getStart()->getLine());
    } else
        leftExpr = (ASTNode *) visit(ctx->left);

    return (ASTNode *) new IntervalNode(leftExpr, rightExpr, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitByExpr(gazprea::GazpreaParser::ByExprContext *ctx) {
    return (ASTNode *) new ByNode(visit(ctx->left), visit((ctx->right)), (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitIndexExpr(gazprea::GazpreaParser::IndexExprContext *ctx) {
    auto *expr  = new std::vector<ASTNode *>;

    for(unsigned long i = 0; i < ctx->expr().size(); ++i){
        expr->push_back((ASTNode *) visit(ctx->expr(i)));
    }

    return (ASTNode *) new IndexNode(expr, (int)ctx->expr().size(),(int)ctx->getStart()->getLine());
}

/**
 * Unary Expr
 *
 * @param ctx
 * @return - NegateNode or generated subtraction;
 */
antlrcpp::Any ASTGenerator::visitUnaryExpr(gazprea::GazpreaParser::UnaryExprContext *ctx) {
    ASTNode * expr = (ASTNode *) visit(ctx->expr());
    if     (ctx->SUB()){
        auto * zero = (ASTNode *) new INTNode(0, (int) ctx->getStart()->getLine());
        return (ASTNode *) new SubNode(zero, expr, (int)ctx->getStart()->getLine());
    }
    else if(ctx->NOT()){
        return (ASTNode *) new NegateNode(expr, (int)ctx->getStart()->getLine());
    }

    return expr;
}

/**
 * Identifier
 *  - literal string for variable definition
 *  - createsa global node if the identifier is global
 * @param ctx
 * @return -
 */
antlrcpp::Any ASTGenerator::visitIdentifierExpr(gazprea::GazpreaParser::IdentifierExprContext *ctx) {
    std::string id = ctx->Identifier()->getText();
    auto globalVar = globalVars->find(id);

    //check if global
    if (not(globalVar == globalVars->end()))
        return (ASTNode *) new GlobalRefNode(id, (int)ctx->getStart()->getLine());
    IDNode *idNode = new IDNode(id, (int)ctx->getStart()->getLine());
    return (ASTNode *) idNode;
}

/**
 * Identity
 *  - reserved for the identity object
 * @param ctx
 * @return - IdnNode
 */
antlrcpp::Any ASTGenerator::visitIdentityExpr(gazprea::GazpreaParser::IdentityExprContext *ctx) {
    return (ASTNode *) new IdnNode((int)ctx->getStart()->getLine());
}

/**
 * visits the statement
 * @param ctx
 * @return - ASTNode from the visit
 */
antlrcpp::Any ASTGenerator::visitStatement(gazprea::GazpreaParser::StatementContext *ctx) {
    return GazpreaBaseVisitor::visitStatement(ctx);
}

/**
 * Normal assignment
 *  - assigns left to right
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitNormalAss(gazprea::GazpreaParser::NormalAssContext *ctx) {
    ASTNode *expr = nullptr;
    if(ctx->expr())
        expr = (ASTNode *) visit(ctx->expr());
    return (ASTNode *) new AssignNode(expr, ctx->Identifier()->getText(), (int)ctx->getStart()->getLine());
}

/**
 * If statements
 *  - must need all of the conditions and all of the blocks
 * @param ctx
 * @return - CondNode
 */
antlrcpp::Any ASTGenerator::visitConditional(gazprea::GazpreaParser::ConditionalContext *ctx) {
    auto *conds  = new std::vector<ASTNode *>;
    auto *blocks = new std::vector<ASTNode *>;

    for(unsigned long i = 0; i < ctx->expr().size(); ++i){
        conds->push_back((ASTNode *) visit(ctx->expr().at(i)));
    }
    for(unsigned long i = 0; i < ctx->block().size(); ++i){
        blocks->push_back((ASTNode *) visit(ctx->block().at(i)));
    }

    return (ASTNode *) new CondNode(conds, blocks, (int)ctx->getStart()->getLine());
}

/**
 * Loop with no control
 *
 * @param ctx
 * @return - LoopNode with true condition
 */
antlrcpp::Any ASTGenerator::visitInfiniteLoop(gazprea::GazpreaParser::InfiniteLoopContext *ctx) {
    ASTNode * t = (ASTNode *) new BoolNode(true, (int)ctx->getStart()->getLine());
    return (ASTNode *) new LoopNode((ASTNode *) visit(ctx->block()), t, (int)ctx->getStart()->getLine());
}

/**
 * Loop with control
 *
 * @param ctx
 * @return - LoopNode with visited condition
 */
antlrcpp::Any ASTGenerator::visitPredicatedLoop(gazprea::GazpreaParser::PredicatedLoopContext *ctx) {
    return (ASTNode *) new LoopNode((ASTNode *) visit(ctx->block()), (ASTNode *) visit(ctx->expr()), (int)ctx->getStart()->getLine());
}

/**
 * Loop with control at the end
 * @param ctx
 * @return - LoopNode with visited condition, body at the top
 */
antlrcpp::Any ASTGenerator::visitDoLoop(gazprea::GazpreaParser::DoLoopContext *ctx) {
    return (ASTNode *) new DoLoopNode((ASTNode *) visit(ctx->block()), (ASTNode *) visit(ctx->expr()), (int)ctx->getStart()->getLine());
}

/**
 * this is for next project
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitIteratorLoop(gazprea::GazpreaParser::IteratorLoopContext *ctx) {
    return GazpreaBaseVisitor::visitIteratorLoop(ctx);
}

/**
 * Block
 *  - builds block
 *  - visits the declarations and body block
 *  - builds the remaining full block
 *
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitBlock(gazprea::GazpreaParser::BlockContext *ctx) {
    ASTNode *declBlock;
    ASTNode *bodyBlock;

    if(ctx->decBlock())
        declBlock = (ASTNode *)visit(ctx->decBlock());
    else
        declBlock = new BasicBlockNode(new std::vector<ASTNode *>, (int)ctx->getStart()->getLine());

    if(ctx->bodyBlock())
        bodyBlock = (ASTNode *) visit(ctx->bodyBlock());
    else
        bodyBlock = new BasicBlockNode(new std::vector<ASTNode *>, (int)ctx->getStart()->getLine());

    if(ctx->single_statement()){
        ASTNode * stmt = (ASTNode *) visit(ctx->single_statement());
        auto *single = new std::vector<ASTNode *>;
        single->push_back(stmt);
        bodyBlock = new BasicBlockNode(single, (int)ctx->getStart()->getLine());
    }

    return (ASTNode *) new BlockNode(declBlock, bodyBlock, (int)ctx->getStart()->getLine());
}

/**
 * Visit of a declaration block
 * @param ctx
 * @return - BasicBlock for the declarations
 */
antlrcpp::Any ASTGenerator::visitDecBlock(gazprea::GazpreaParser::DecBlockContext *ctx) {
    auto *statements = new std::vector<ASTNode *>;
    for(unsigned long i = 0;i < ctx->declaration().size();++i){
        statements->push_back( (ASTNode *) visit(ctx->declaration()[i]));
    }
    return (ASTNode *) new BasicBlockNode(statements, (int)ctx->getStart()->getLine());
}

/**
 * Visit of a body block
 *  - must not have declarations
 * @param ctx
 * @return - BasicBlock for the statements
 */
antlrcpp::Any ASTGenerator::visitBodyBlock(gazprea::GazpreaParser::BodyBlockContext *ctx) {
    auto *statements = new std::vector<ASTNode *>;
    for(unsigned long i=0;i < ctx->statement().size(); ++i){
        ASTNode * node = (ASTNode *) visit(ctx->statement()[i]);
        if (dynamic_cast<DeclNode *>(node)) {
            std::cerr << "Declaration does not precede procedure body: " << ctx->statement()[i]->getText() << "\n";
            std::cerr << "Aborting...\n";
            exit(1);
        }
        statements->push_back( node );
        if((dynamic_cast<BreakNode *>(node)) || (dynamic_cast<ContinueNode *>(node))){
            break;
        }
    }
    return (ASTNode *) new BasicBlockNode(statements, (int)ctx->getStart()->getLine());
}

/**
 * Print to out
 * @param ctx
 * @return - OutPutNode
 */
antlrcpp::Any ASTGenerator::visitOutStream(gazprea::GazpreaParser::OutStreamContext *ctx) {
    ASTNode * expr = (ASTNode *) visit(ctx->expr());
    return (ASTNode *) new OutputNode(ctx->Identifier()->getText(), expr, (int)ctx->getStart()->getLine());
}

/**
 * Read from in
 * @param ctx
 * @return - InputNode
 */
antlrcpp::Any ASTGenerator::visitInStream(gazprea::GazpreaParser::InStreamContext *ctx) {
    if(ctx->TupleIndex()){
        std::string tupleText = ctx->TupleIndex()->getText();
        std::vector<std::string> values = split(tupleText,'.');
        std::string idName = values[0];

        int lineNum = (int)ctx->getStart()->getLine();
        auto idNode =  (ASTNode *) new IDNode(idName, lineNum);
        ASTNode *index = getIndexNode(values, 1,lineNum);

        auto LHS = (ASTNode *) new IndexTupleNode(index, dynamic_cast<IDNode*>(idNode), (int)ctx->getStart()->getLine());
        return (ASTNode *) new TupleInputNode(ctx->Identifier().at(0)->getText(), LHS, (int)ctx->getStart()->getLine());
    }
    return (ASTNode *) new InputNode(ctx->Identifier().at(1)->getText(), ctx->Identifier().at(0)->getText(), (int)ctx->getStart()->getLine());
}

/**
 * TypeDefine
 *
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitTypeDefine(gazprea::GazpreaParser::TypeDefineContext *ctx) {
    std::string type = ctx->type()->getText();
    std::string id = ctx->Identifier()->getText();

    if(ctx->type()->tupleType()) {
        ASTNode *tuple = (ASTNode *) visit(ctx->type()->tupleType());
        return (ASTNode *) new TypeDefNode(id, tuple, (int)ctx->getStart()->getLine());
    }

    return (ASTNode *) new TypeDefNode(id, type, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitProcedureCall(gazprea::GazpreaParser::ProcedureCallContext *ctx) {
    if(inFunction) {
        auto ret = functionNames->find(ctx->Identifier()->getText());
        assert(ret != functionNames->end());

    } else {
        auto ret = procedureNames->find(ctx->Identifier()->getText());
        assert(ret != procedureNames->end());
    }

    auto *exprNodes = new std::vector<ASTNode*>;
    for(unsigned long i = 0; i < ctx->expr().size(); ++i) {
        ASTNode * node = (ASTNode *) visit(ctx->expr()[i]);
        exprNodes->push_back(node);
    }
    return (ASTNode *) new CallNode(exprNodes, ctx->Identifier()->getText(), (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitType(gazprea::GazpreaParser::TypeContext *ctx) {
    return GazpreaBaseVisitor::visitType(ctx);
}

antlrcpp::Any ASTGenerator::visitProcedure(gazprea::GazpreaParser::ProcedureContext *ctx) {
    auto ret = procedureNames->find(ctx->Identifier()->getText());
    if(ret == procedureNames->end())
        procedureNames->insert(ctx->Identifier()->getText());
    std::string retType = "void";
    TupleType * tupleType = nullptr;

    if(ctx->returnStat() && ctx->returnStat()->type()->tupleType()){
        tupleType = (TupleType *) (ASTNode *) visit(ctx->returnStat()->type()->tupleType());
    }
    if(ctx->returnStat()) retType = ctx->returnStat()->type()->getText();
    BlockNode *block  = (BlockNode *) (ASTNode *) visit(ctx->block());
    std::vector<ASTNode *> *params = (std::vector<ASTNode *> *) visit(ctx->params());
    ASTNode * p = (ASTNode *) new ProcedureNode(block, params, retType, ctx->Identifier()->getText(), (int)ctx->getStart()->getLine(), tupleType);
    return p;
}

ASTGenerator::ASTGenerator() {
    globalVars     = new std::unordered_set<std::string>();
    functionNames  = new std::unordered_set<std::string>();
    procedureNames = new std::unordered_set<std::string>();
    symbolTable    = new SymbolTable();
}

/**
 * TODO - this
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitParams(gazprea::GazpreaParser::ParamsContext *ctx) {
    //returns a vector or parameter nodes
    auto *paramVec = new std::vector<ASTNode *>;
    for (unsigned long i = 0; i < ctx->param().size(); ++i){
        bool constant = nullptr == ctx->param().at(i)->VAR();
        if (ctx->param().at(i)->type()->tupleType()) {
            TupleType *tt = (TupleType *) ((ASTNode *) visit(ctx->param().at(i)->type()->tupleType()));
            paramVec->push_back(new ParamNode(ctx->param().at(i)->type()->getText(),
                                              ctx->param().at(i)->Identifier()->getText(), constant,
                                              (int)ctx->getStart()->getLine(), tt));
        } else{
            paramVec->push_back(new ParamNode(ctx->param().at(i)->type()->getText(),
                                              ctx->param().at(i)->Identifier()->getText(), constant, (int)ctx->getStart()->getLine()));
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

antlrcpp::Any ASTGenerator::visitBoolExpr(gazprea::GazpreaParser::BoolExprContext *ctx) {
    std::string strExpr = ctx->getText();
    if(strExpr == "true") {
        bool val = true;
        return (ASTNode *) new BoolNode(val, (int)ctx->getStart()->getLine());
    }
    else if(strExpr == "false") {
        bool val = false;
        return (ASTNode *) new BoolNode(val, (int)ctx->getStart()->getLine());
    }
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitStreamDecl(gazprea::GazpreaParser::StreamDeclContext *ctx) {
    assert(not(inFunction));
    int type;
    if(ctx->STD_INPUT())
        type = INSTREAM;
    else
        type = OUTSTREAM;
    return (ASTNode *) new StreamDeclNode(ctx->Identifier()->getText(), type, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitNormalDecl(gazprea::GazpreaParser::NormalDeclContext *ctx) {
    int line = (int)ctx->getStart()->getLine();
    ASTNode *expr = nullptr;
    if(ctx->expr()){
        expr = (ASTNode *) visit(ctx->expr());
    }

    bool constant = (nullptr != ctx->CONST());
    std::string id = ctx->Identifier()->getText();
    std::string ty;
    if (!ctx->type().empty()) ty = ctx->type(0)->getText();

    // todo string decl, interval
    // TODO if user defined a type called xxxvector then it would go to vector, same for interval and matrix
    // if decl is a tuple decl then
    if ((ctx->type().size() == 1) && (ty.substr(0, 6) == "tuple(")) {
        return (ASTNode *) new TupleDeclNode(expr, constant, id, visit(ctx->type(0)), line);
    }
    // if matrix decl then
    else if ((ctx->type().size() == 1 && ty.size() > 6) && (ty.substr(ty.size() - 6) == "matrix")) {
        if (ctx->extension() == nullptr) {  // if extension not provided
            return (ASTNode *) new MatrixDeclNode(expr, line, constant, id, ty, nullptr);
        }
        return (ASTNode *) new MatrixDeclNode(expr, line, constant, id, ty, visit(ctx->extension()));
    }
    // TODO: Parse vector type and add it as a string to vectorDeclNode
    // if vector decl then
    else if ((ctx->type().size() == 1 && ty.size() > 6) && (ty.substr(ty.size() - 6) == "vector")) {
        if (ctx->extension() == nullptr) {
            return (ASTNode *) new VectorDeclNode(expr, constant, id, ty, nullptr, line);
        }
        return (ASTNode *) new VectorDeclNode(expr, constant, id, ty, visit(ctx->extension()), line);
    }
    // if interval decl then
    else if (ctx->type().size() == 1 && ty.size() > 8 && ty.substr(ty.size() - 8) == "interval") {
        return (ASTNode *) new IntervalDeclNode(expr, constant, id, line);
    }
    else { // else it's a normal decl
        auto *typeVec = new std::vector<std::string>();

        for (unsigned long i = 0; i < ctx->type().size(); ++i) {
            typeVec->push_back(ctx->type().at(i)->getText());
        }
        return (ASTNode *) new DeclNode(expr, constant, id, typeVec, expr->getType(), line);
    }
}

antlrcpp::Any ASTGenerator::visitCharExpr(gazprea::GazpreaParser::CharExprContext *ctx) {
    // char will be one character inside a single quotation so we just need to get the second element
    char val = ctx->getText().at(1);
    std::string trueVal = ctx->getText();

    if (trueVal[1] == '\\') {
        if (trueVal[2] == 'n' ) {
            val = 0x0A;
        } else if (trueVal[2] == '0' ) {
            val = 0x00;
        } else if (trueVal[2] == 'a' ) {
            val = 0x07;
        } else if (trueVal[2] == 'b' ) {
            val = 0x08;
        } else if (trueVal[2] == 't' ) {
            val = 0x09;
        } else if (trueVal[2] == 'r' ) {
            val = 0x0D;
        } else if (trueVal[2] == '"' ) {
            val = 0x22;
        } else if (trueVal[2] == '\'' ) {
            val = 0x27;
        } else if (trueVal[2] == '\\' ) {
            val = 0x5C;
        }
    }

    return (ASTNode *) new CharNode(val, (int)ctx->getStart()->getLine());
}

/*
 * ignore the warning on dynamic cast
 */
antlrcpp::Any ASTGenerator::visitTupleIndexExpr(gazprea::GazpreaParser::TupleIndexExprContext *ctx) {
    std::string tupleText = ctx->TupleIndex()->getText();
    std::vector<std::string> values = split(tupleText,'.');
    std::string idName = values[0];

    int lineNum = (int)ctx->getStart()->getLine();
    auto idNode = (ASTNode *) new IDNode(idName, lineNum);
    ASTNode *index = getIndexNode(values, 1, lineNum);

    return (ASTNode *) new IndexTupleNode(index, dynamic_cast<IDNode*>(idNode), (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitPythonTupleAss(gazprea::GazpreaParser::PythonTupleAssContext *ctx) {
    ASTNode *expr = nullptr;
    if(ctx->expr())
        expr = (ASTNode *) visit(ctx->expr());

    auto *IDs = new std::vector<std::string>;
    for (auto element : ctx->Identifier()) {
        IDs->push_back(element->getText());
    }

    return (ASTNode *) new PythonTupleAssNode(expr, *IDs, (int)ctx->getStart()->getLine());
}

// this just calls visitTuple
antlrcpp::Any ASTGenerator::visitTupleExpr(gazprea::GazpreaParser::TupleExprContext *ctx) {
    return (ASTNode *) GazpreaBaseVisitor::visitTupleExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitTuple(gazprea::GazpreaParser::TupleContext *ctx) {
    auto *expr  = new std::vector<ASTNode *>;
    for(unsigned long i = 0; i < ctx->expr().size(); ++i){
        expr->push_back((ASTNode *) visit(ctx->expr(i)));
    }
    return (ASTNode *) new TupleNode(expr, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitTupleType(gazprea::GazpreaParser::TupleTypeContext *ctx) {
    auto *typeIdNodes = new std::vector<ASTNode *>;

    for (auto typeId : ctx->tupleTypeIdentifier()) {
        typeIdNodes->push_back((ASTNode *) visit(typeId));
    }

    return (ASTNode *) new TupleType(typeIdNodes, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitVectorType(gazprea::GazpreaParser::VectorTypeContext *ctx) {
    if (ctx->extension() == nullptr) {
        if (ctx->getText().at(0) == 'b')
            return (ASTNode *) new VectorType(nullptr, "booleanvector", (int)ctx->getStart()->getLine());
        else if (ctx->getText().at(0) == 'i')
            return (ASTNode *) new VectorType(nullptr, "integervector", (int)ctx->getStart()->getLine());
        else if (ctx->getText().at(0) == 'c')
            return (ASTNode *) new VectorType(nullptr, "charactervector", (int)ctx->getStart()->getLine());
        else if (ctx->getText().at(0) == 'r')
            return (ASTNode *) new VectorType(nullptr, "realvector", (int)ctx->getStart()->getLine());

    } else {
        if (ctx->getText().at(0) == 'b')
            return (ASTNode *) new VectorType(visit(ctx->extension()), "booleanvector", (int)ctx->getStart()->getLine());
        else if (ctx->getText().at(0) == 'i')
            return (ASTNode *) new VectorType(visit(ctx->extension()), "integervector", (int)ctx->getStart()->getLine());
        else if (ctx->getText().at(0) == 'c')
            return (ASTNode *) new VectorType(visit(ctx->extension()), "charactervector", (int)ctx->getStart()->getLine());
        else if (ctx->getText().at(0) == 'r')
            return (ASTNode *) new VectorType(visit(ctx->extension()), "realvector", (int)ctx->getStart()->getLine());
    }
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitMatrixType(gazprea::GazpreaParser::MatrixTypeContext *ctx) {
    return nullptr;
}

// everything inside the tuple() is a empty decl
antlrcpp::Any ASTGenerator::visitTupleTypeIdentifier(gazprea::GazpreaParser::TupleTypeIdentifierContext *ctx) {
    ASTNode *expr = (ASTNode *) new IDNode("null", (int)ctx->getStart()->getLine());
    bool constant = false;

    auto *typeVec = new std::vector<std::string>();
    typeVec->push_back(ctx->type()->getText());

    if (ctx->Identifier() == nullptr) {
        return (ASTNode *) new DeclNode(expr, constant, "", typeVec, expr->getType(), (int)ctx->getStart()->getLine());
    } else {
        return (ASTNode *) new DeclNode(expr, constant, ctx->Identifier()->getText(), typeVec, expr->getType(), (int)ctx->getStart()->getLine());
    }
}

antlrcpp::Any ASTGenerator::visitEmptyDecl(gazprea::GazpreaParser::EmptyDeclContext *ctx) {
    auto *expr = (ASTNode *) new NullNode((int)ctx->getStart()->getLine());

    bool constant = (nullptr != ctx->CONST());
    std::string id = ctx->Identifier()->getText();
    std::string ty;
    if (!ctx->type().empty())  ty = ctx->type(0)->getText();

    // if decl is a tuple decl then
    if ((ctx->type().size() == 1) && (ty.substr(0, 6) == "tuple(")) {
        return (ASTNode *) new TupleDeclNode(expr, constant, id, visit(ctx->type(0)), (int)ctx->getStart()->getLine());
    }
    else { // else it's a normal decl
        auto *typeVec = new std::vector<std::string>();
        for (unsigned long i = 0; i < ctx->type().size(); ++i) {
            typeVec->push_back(ctx->type().at(i)->getText());
        }

        return (ASTNode *) new DeclNode(expr, constant, id, typeVec, expr->getType(), (int)ctx->getStart()->getLine());
    }
}

antlrcpp::Any ASTGenerator::visitContinueStat(gazprea::GazpreaParser::ContinueStatContext *ctx) {
    return (ASTNode *) new ContinueNode((int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitBreakStat(gazprea::GazpreaParser::BreakStatContext *ctx) {
    return (ASTNode *) new BreakNode((int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitStreamAss(gazprea::GazpreaParser::StreamAssContext *ctx) {
    ASTNode * right;
    if(ctx->STD_INPUT()){
        right = (ASTNode *) new IDNode("std_input()", (int)ctx->getStart()->getLine());
    } else {
        right = (ASTNode *) new IDNode("std_output()", (int)ctx->getStart()->getLine());
    }

    return (ASTNode *) new AssignNode(right, ctx->Identifier()->getText(), (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitGlobalDecl(gazprea::GazpreaParser::GlobalDeclContext *ctx) {
    ASTNode *expr  = (ASTNode *) visit(ctx->expr());
    std::string id = ctx->Identifier()->getText();
    auto *typeVec  = new std::vector<std::string>();

    for (unsigned long i = 0; i < ctx->type().size(); ++i) {
        typeVec->push_back(ctx->type().at(i)->getText());
    }
    globalVars->insert(id);
    return (ASTNode *) new GlobalDeclNode(expr, id, typeVec, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitTupleMemberAss(gazprea::GazpreaParser::TupleMemberAssContext *ctx) {
    std::string tupleText = ctx->TupleIndex()->getText();
    std::vector<std::string> values = split(tupleText,'.');
    std::string idName = values[0];
    ASTNode *expr  = (ASTNode *) visit(ctx->expr());

    int lineNum = (int)ctx->getStart()->getLine();
    auto idNode =  (ASTNode*) new IDNode(idName, lineNum);
    ASTNode *index = getIndexNode(values, 1, lineNum);

    auto LHS = new IndexTupleNode(index, dynamic_cast<IDNode*>(idNode), (int)ctx->getStart()->getLine());

   return (ASTNode *) new TupleMemberAssNode(expr, LHS, (int)ctx->getStart()->getLine());
}

/**
 * same as procedure but with required type
 *
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitFunction(gazprea::GazpreaParser::FunctionContext *ctx) {
    inFunction = true;
    TupleType * tupleType = nullptr;
    if(ctx->type()->tupleType()){
        tupleType = (TupleType *) (ASTNode *) visit(ctx->type()->tupleType());
    }
    auto ret = functionNames->find(ctx->Identifier()->getText());
    if(ret == functionNames->end())
        functionNames->insert(ctx->Identifier()->getText());
    std::string retType = ctx->type()->getText();
    BlockNode *block  = (BlockNode *) (ASTNode *) visit(ctx->functionReturns());
    std::vector<ASTNode *> *params = (std::vector<ASTNode *> *) visit(ctx->params());
    ASTNode * f = (ASTNode *) new FunctionNode(block, params, retType, ctx->Identifier()->getText(),
            (int)ctx->getStart()->getLine(), tupleType);

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
    if(ctx->expr()){
        ASTNode * expr    = (ASTNode *) visit(ctx->expr());
        ASTNode * retNode = new ReturnNode(expr,  (int)ctx->getStart()->getLine());
        auto *declBlock = new std::vector<ASTNode *>();
        auto *statBlock = new std::vector<ASTNode *>();
        auto *declBlockNode = new BasicBlockNode(declBlock, (int)ctx->getStart()->getLine());
        auto *statBlockNode = new BasicBlockNode(statBlock, (int)ctx->getStart()->getLine());
        statBlock->push_back(retNode);
        blockNode = (ASTNode *) new BlockNode(declBlockNode, statBlockNode, (int)ctx->getStart()->getLine());
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
    auto ret = procedureNames->find(ctx->Identifier()->getText());
    if(ret == procedureNames->end())
        procedureNames->insert(ctx->Identifier()->getText());
    std::string retType = "void";
    TupleType * tupleType = nullptr;
    if(ctx->returnStat() && ctx->returnStat()->type()->tupleType()){
        tupleType = (TupleType *) (ASTNode *) visit(ctx->returnStat()->type()->tupleType());
    }
    if(ctx->returnStat()) retType = ctx->returnStat()->type()->getText();
    std::vector<ASTNode *> *params = (std::vector<ASTNode *> *) visit(ctx->params());
    ASTNode * p = (ASTNode *) new ProtoProcedureNode(params, retType, ctx->Identifier()->getText(),
            (int)ctx->getStart()->getLine(), tupleType);
    return p;
}

antlrcpp::Any ASTGenerator::visitFuncProto(gazprea::GazpreaParser::FuncProtoContext *ctx) {
    auto ret = functionNames->find(ctx->Identifier()->getText());
    if(ret == functionNames->end())
        functionNames->insert(ctx->Identifier()->getText());
    std::string retType = ctx->returnStat()->type()->getText();
    TupleType * tupleType = nullptr;
    if(ctx->returnStat()->type()->tupleType()){
        tupleType = (TupleType *) (ASTNode *) visit(ctx->returnStat()->type()->tupleType());
    }
    std::vector<ASTNode *> *params = (std::vector<ASTNode *> *) visit(ctx->params());
    ASTNode * p = (ASTNode *) new ProtoProcedureNode(params, retType, ctx->Identifier()->getText(),
            (int)ctx->getStart()->getLine(), tupleType);
    return p;
}

antlrcpp::Any ASTGenerator::visitFunctionCall(gazprea::GazpreaParser::FunctionCallContext *ctx) {
    std::string functionName = ctx->Identifier()->getText();

    // Checks if in a function
    if(inFunction) {
        auto ret = functionNames->find(ctx->Identifier()->getText());
        assert(ret != functionNames->end());
    }

    auto *exprNodes = new std::vector<ASTNode*>;
    for(unsigned long i = 0; i < ctx->expr().size(); ++i) {
        ASTNode * node = (ASTNode *) visit(ctx->expr()[i]);
        exprNodes->push_back(node);
    }
    return (ASTNode *) new CallNode(exprNodes, functionName, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitArithExpr(gazprea::GazpreaParser::ArithExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    if (ctx->op->getType() == gazprea::GazpreaParser::ADD) {
        return (ASTNode *) new AddNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::SUB) {
        return (ASTNode *) new SubNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::MUL) {
        return (ASTNode *) new MulNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::DIV) {
        return (ASTNode *) new DivNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::REM) {
        return (ASTNode *) new RemNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::EXP) {
        return (ASTNode *) new ExpNode(left, right, (int)ctx->getStart()->getLine());
    }
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitCompExpr(gazprea::GazpreaParser::CompExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    if(ctx->op->getType() == gazprea::GazpreaParser::LESST){
        return (ASTNode *) new LTNode(left, right,(int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::LESSTE){
        return (ASTNode *) new LTENode(left, right,(int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::MORET){
        return (ASTNode *) new GTNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::MORETE){
        return (ASTNode *) new GTENode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::AND) {
        return (ASTNode *) new AndNode(left, right, (int) ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::OR){
        return (ASTNode *) new OrNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::XOR){
        return (ASTNode *) new XOrNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::EEQL){
        return (ASTNode *) new EQNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::NEQL){
        return (ASTNode *) new NEQNode(left, right, (int)ctx->getStart()->getLine());
    }
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitVectorExpr(gazprea::GazpreaParser::VectorExprContext *ctx) {
    auto *expr  = new std::vector<ASTNode *>;

    for(unsigned long i = 0; i < ctx->vector()->expr().size(); ++i){
        expr->push_back((ASTNode *) visit(ctx->vector()->expr(i)));
    }

    return (ASTNode *) new VectorNode(expr, (int)ctx->getStart()->getLine());
}

// this just calls visitMatrix
antlrcpp::Any ASTGenerator::visitMatrixExpr(gazprea::GazpreaParser::MatrixExprContext *ctx) {
    return GazpreaBaseVisitor::visitMatrixExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitMatrix(gazprea::GazpreaParser::MatrixContext *ctx) {
    auto *expr  = new std::vector<ASTNode *>;
    for(auto vector : ctx->vector()){
        expr->push_back((ASTNode *) visit(vector));
    }
    return (ASTNode *) new MatrixNode(expr, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitExtension(gazprea::GazpreaParser::ExtensionContext *ctx) {

    // if it is a [] of size 2
    if (ctx->rightExtension()) {
        ASTNode *left;

        if (ctx->left == nullptr) {         // being nullptr assumes it's actually a *
            left = nullptr;
        } else {
            left = visit(ctx->left);
        }

        return (ASTNode *) new MatrixType(left, visit(ctx->rightExtension()), (int) ctx->getStart()->getLine());
    }

    // if it is a [] of size 1
    else if (!ctx->rightExtension()) {
        ASTNode *size;

        if(ctx->left == nullptr) {
            size = nullptr;
        } else {
            size = visit(ctx->left);
        }

        return size;
    }

    return nullptr;
}

antlrcpp::Any ASTGenerator::visitRightExtension(gazprea::GazpreaParser::RightExtensionContext *ctx) {
    if(ctx->right == nullptr)
        return nullptr;
    else
        return visit(ctx->right);
}

antlrcpp::Any ASTGenerator::visitStringExpr(gazprea::GazpreaParser::StringExprContext *ctx) {
    int line = (int)ctx->getStart()->getLine();
    auto str = ctx->getText();
    auto strVec = new std::vector<ASTNode *>;

    for (int i = 1; i < (int) str.size() - 1; i++){
        if (str[i] != '\\') {
            strVec->push_back((ASTNode *) new CharNode(str[i], line));
        }
        else {
            i = i + 1;
            if (str[i] == 'n' ) {
                strVec->push_back((ASTNode *) new CharNode(0x0A, line));
            } else if (str[i] == '0' ) {
                strVec->push_back((ASTNode *) new CharNode(0x00, line));
            } else if (str[i] == 'a' ) {
                strVec->push_back((ASTNode *) new CharNode(0x07, line));
            } else if (str[i] == 'b' ) {
                strVec->push_back((ASTNode *) new CharNode(0x08, line));
            } else if (str[i] == 't' ) {
                strVec->push_back((ASTNode *) new CharNode(0x09, line));
            } else if (str[i] == 'r' ) {
                strVec->push_back((ASTNode *) new CharNode(0x0D, line));
            } else if (str[i] == '"' ) {
                strVec->push_back((ASTNode *) new CharNode(0x22, line));
            } else if (str[i] == '\'' ) {
                strVec->push_back((ASTNode *) new CharNode(0x27, line));
            } else if (str[i] == '\\' ) {
                strVec->push_back((ASTNode *) new CharNode(0x5C, line));
            } else {
                std::cerr << "Unrecognized character\nAborting....";
                exit(1);
            }
        }
    }
    return (ASTNode *) new VectorNode(strVec, line);        // todo maybe shouldnt be a vector node
}

antlrcpp::Any ASTGenerator::visitGeneratorExpr(gazprea::GazpreaParser::GeneratorExprContext *ctx) {
    return GazpreaBaseVisitor::visitGeneratorExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitFilterExpr(gazprea::GazpreaParser::FilterExprContext *ctx) {
    return GazpreaBaseVisitor::visitFilterExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitVectorLengthExpr(gazprea::GazpreaParser::VectorLengthExprContext *ctx) {
    return GazpreaBaseVisitor::visitVectorLengthExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitRowLengthExpr(gazprea::GazpreaParser::RowLengthExprContext *ctx) {
    return GazpreaBaseVisitor::visitRowLengthExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitColLengthExpr(gazprea::GazpreaParser::ColLengthExprContext *ctx) {
    return GazpreaBaseVisitor::visitColLengthExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitReverseExpr(gazprea::GazpreaParser::ReverseExprContext *ctx) {
    return GazpreaBaseVisitor::visitReverseExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitDotProductExpr(gazprea::GazpreaParser::DotProductExprContext *ctx) {
    return GazpreaBaseVisitor::visitDotProductExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitConcatExpr(gazprea::GazpreaParser::ConcatExprContext *ctx) {
    return GazpreaBaseVisitor::visitConcatExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitStreamState(gazprea::GazpreaParser::StreamStateContext *ctx) {
    std::string streamName = ctx->Identifier()->getText();
    return (ASTNode *) new StreamStateNode((int)ctx->getStart()->getLine(),streamName);
}
