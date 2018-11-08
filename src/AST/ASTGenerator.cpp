//
// Created by kyle on 20/10/18.
//
#include <AST/ASTGenerator.h>

antlrcpp::Any ASTGenerator::visitFile(gazprea::GazpreaParser::FileContext *ctx) {
    auto *procedures = new std::vector<ASTNode *>;
    unsigned long i;
    for (i = 0; i < ctx->termsAndConditions().size(); i++){
        ASTNode * p = (ASTNode *) visit(ctx->termsAndConditions().at(i));
        if (nullptr == p){
            printf("yes\n");
        }
        procedures->push_back(p);
    }
    return (ASTNode *) new FileNode(procedures, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitExponentExpr(gazprea::GazpreaParser::ExponentExprContext *ctx) {
    ASTNode * left = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);

    return (ASTNode *) new ExpNode(left, right, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitIntegerExpr(gazprea::GazpreaParser::IntegerExprContext *ctx) {
    std::string rawStr = ctx->getText();
    rawStr.erase(std::remove(rawStr.begin(), rawStr.end(), '_'), rawStr.end());     // remove the underscores
    int val = std::stoi(rawStr);
    return (ASTNode *) new INTNode(val, (int) ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitNullExpr(gazprea::GazpreaParser::NullExprContext *ctx) {
    return (ASTNode *) new NullNode((int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitCastExpr(gazprea::GazpreaParser::CastExprContext *ctx) {
    ASTNode * expr = (ASTNode *) visit(ctx->expr());

    if(ctx->type()->tupleType()) {
        ASTNode *tuple = (ASTNode *) visit(ctx->type()->tupleType());
        return (ASTNode *) new CastExprNode(expr, tuple, (int)ctx->getStart()->getLine());
    }

    return (ASTNode *) new CastExprNode(expr, ctx->type()->getText(), (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitRealExpr(gazprea::GazpreaParser::RealExprContext *ctx) {
    std::string strVal = ctx->getText();
    std::string str2Val;

    std::copy_if (strVal.begin(), strVal.end(), std::back_inserter(str2Val), [](char i){return i != '_';} ); // remove _
    float val = std::stof(str2Val);
    return (ASTNode *) new RealNode(val, (int)ctx->getStart()->getLine());}

antlrcpp::Any ASTGenerator::visitBrackExpr(gazprea::GazpreaParser::BrackExprContext *ctx) {
    return (ASTNode *) visit(ctx->expr());
}

antlrcpp::Any ASTGenerator::visitLessExpr(gazprea::GazpreaParser::LessExprContext *ctx) {
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
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitEqlExpr(gazprea::GazpreaParser::EqlExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    if(ctx->op->getType() == gazprea::GazpreaParser::EEQL){
        return (ASTNode *) new EQNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::NEQL){
        return (ASTNode *) new NEQNode(left, right, (int)ctx->getStart()->getLine());
    }
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitOrExpr(gazprea::GazpreaParser::OrExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    if(ctx->op->getType() == gazprea::GazpreaParser::OR){
        return (ASTNode *) new OrNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::XOR){
        return (ASTNode *) new XOrNode(left, right, (int)ctx->getStart()->getLine());
    }
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitDomainExpr(gazprea::GazpreaParser::DomainExprContext *ctx) {
    return GazpreaBaseVisitor::visitDomainExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitIndexExpr(gazprea::GazpreaParser::IndexExprContext *ctx) {
    return GazpreaBaseVisitor::visitIndexExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitUnaryExpr(gazprea::GazpreaParser::UnaryExprContext *ctx) {
    ASTNode * expr = (ASTNode *) visit(ctx->expr());
    if     (ctx->SUB()){
        auto * zero = (ASTNode *) new INTNode(0, (int) ctx->getStart()->getLine());
        return (ASTNode *) new SubNode(zero, expr, (int)ctx->getStart()->getLine());
    }
    else if(ctx->NOT()){
        return (ASTNode *) new NegateNode(expr, (int)ctx->getStart()->getLine());
    }

    return GazpreaBaseVisitor::visitUnaryExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitAddExpr(gazprea::GazpreaParser::AddExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    if(ctx->op->getType() == gazprea::GazpreaParser::ADD) {
        return (ASTNode *) new AddNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::SUB) {
        return (ASTNode *) new SubNode(left, right, (int)ctx->getStart()->getLine());
    }
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitMulExpr(gazprea::GazpreaParser::MulExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    if(ctx->op->getType() == gazprea::GazpreaParser::MUL) {
        return (ASTNode *) new MulNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::DIV) {
        return (ASTNode *) new DivNode(left, right, (int)ctx->getStart()->getLine());
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::REM) {
        return (ASTNode *) new RemNode(left, right, (int)ctx->getStart()->getLine());
    }
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitIdentifierExpr(gazprea::GazpreaParser::IdentifierExprContext *ctx) {
    std::string id = ctx->Identifier()->getText();
    auto globalVar = globalVars->find(id);

    //check if global
    if (not(globalVar == globalVars->end()))
        return (ASTNode *) new GlobalRefNode(id, (int)ctx->getStart()->getLine());

    return (ASTNode *) new IDNode(id, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitIdentityExpr(gazprea::GazpreaParser::IdentityExprContext *ctx) {
    return (ASTNode *) new IdnNode((int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitAndExpr(gazprea::GazpreaParser::AndExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    return (ASTNode *) new AndNode(left, right, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitStatement(gazprea::GazpreaParser::StatementContext *ctx) {
    return GazpreaBaseVisitor::visitStatement(ctx);
}

antlrcpp::Any ASTGenerator::visitNormalAss(gazprea::GazpreaParser::NormalAssContext *ctx) {
    ASTNode *expr = nullptr;
    if(ctx->expr())
        expr = (ASTNode *) visit(ctx->expr());
    return (ASTNode *) new AssignNode(expr, ctx->Identifier()->getText(), (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitConditional(gazprea::GazpreaParser::ConditionalContext *ctx) {
    auto *conds  = new std::vector<ASTNode *>;
    auto *blocks = new std::vector<ASTNode *>;

    unsigned long int i;
    for(i = 0; i < ctx->expr().size(); i++){
        conds->push_back((ASTNode *) visit(ctx->expr().at(i)));
    }
    for(i = 0; i < ctx->block().size(); i++){
        blocks->push_back((ASTNode *) visit(ctx->block().at(i)));
    }

    return (ASTNode *) new CondNode(conds, blocks, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitInfiniteLoop(gazprea::GazpreaParser::InfiniteLoopContext *ctx) {
    return (ASTNode *) new LoopNode((ASTNode *) visit(ctx->block()), (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitPredicatedLoop(gazprea::GazpreaParser::PredicatedLoopContext *ctx) {
    return (ASTNode *) new LoopNode((ASTNode *) visit(ctx->block()), (ASTNode *) visit(ctx->expr()), (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitDoLoop(gazprea::GazpreaParser::DoLoopContext *ctx) {
    return (ASTNode *) new LoopNode((ASTNode *) visit(ctx->block()), (ASTNode *) visit(ctx->expr()), (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitIteratorLoop(gazprea::GazpreaParser::IteratorLoopContext *ctx) {
    return GazpreaBaseVisitor::visitIteratorLoop(ctx);
}

antlrcpp::Any ASTGenerator::visitBlock(gazprea::GazpreaParser::BlockContext *ctx) {
    ASTNode *declBlock;
    ASTNode *bodyBlock;

    if(ctx->decBlock())  declBlock = (ASTNode *) visit(ctx->decBlock());
    else declBlock = new BasicBlockNode(new std::vector<ASTNode *>, (int)ctx->getStart()->getLine());
    if(ctx->bodyBlock()) bodyBlock = (ASTNode *) visit(ctx->bodyBlock());
    else bodyBlock = new BasicBlockNode(new std::vector<ASTNode *>, (int)ctx->getStart()->getLine());
    if(ctx->single_statement()){
        ASTNode * stmt = (ASTNode *) visit(ctx->single_statement());
        auto *single = new std::vector<ASTNode *>;
        single->push_back(stmt);
        bodyBlock = new BasicBlockNode(single, (int)ctx->getStart()->getLine());
    }

    return (ASTNode *) new BlockNode(declBlock, bodyBlock, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitDecBlock(gazprea::GazpreaParser::DecBlockContext *ctx) {
    auto *statements = new std::vector<ASTNode *>;
    unsigned int i;
    for(i=0;i<ctx->declaration().size();i++){
        statements->push_back( (ASTNode *) visit(ctx->declaration()[i]));
    }
    return (ASTNode *) new BasicBlockNode(statements, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitBodyBlock(gazprea::GazpreaParser::BodyBlockContext *ctx) {
    auto *statements = new std::vector<ASTNode *>;
    unsigned int i;
    for(i=0;i<ctx->statement().size(); ++i){
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

antlrcpp::Any ASTGenerator::visitOutStream(gazprea::GazpreaParser::OutStreamContext *ctx) {
    ASTNode * expr = (ASTNode *) visit(ctx->expr());
    return (ASTNode *) new OutputNode(ctx->Identifier()->getText(), expr, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitInStream(gazprea::GazpreaParser::InStreamContext *ctx) {
    if(ctx->tupleMember()){
        std::string idName = ctx->tupleMember()->TupleIndex()->getText();
        idName.erase(std::remove(idName.begin(), idName.end(), '.'), idName.end());

        auto idNode =  new IDNode(idName, (int)ctx->getStart()->getLine());
        ASTNode *index;
        if(ctx->tupleMember()->Integer()){
            int val = std::stoi(ctx->tupleMember()->Integer()->getText());
            assert(val > 0);
            --val;
            index = (ASTNode *) new INTNode(val, (int)ctx->getStart()->getLine());
        }
        else{
            assert(ctx->tupleMember()->Identifier());
            index = (ASTNode *) new IDNode(ctx->tupleMember()->Identifier()->getText(), (int)ctx->getStart()->getLine());
        }

        auto LHS = new IndexTupleNode(index, idNode, (int)ctx->getStart()->getLine());
        return (ASTNode *) new TupleInputNode(ctx->Identifier().at(0)->getText(), LHS, (int)ctx->getStart()->getLine());
    }
    return (ASTNode *) new InputNode(ctx->Identifier().at(1)->getText(), ctx->Identifier().at(0)->getText(), (int)ctx->getStart()->getLine());
}

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
    auto *exprNodes = new std::vector<ASTNode*>;
    for(unsigned int i = 0; i < ctx->expr().size(); ++i) {
        ASTNode * node = (ASTNode *) visit(ctx->expr()[i]);
        exprNodes->push_back(node);
    }
    return (ASTNode *) new CallNode(exprNodes, ctx->Identifier()->getText(), (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitType(gazprea::GazpreaParser::TypeContext *ctx) {
    return GazpreaBaseVisitor::visitType(ctx);
}

antlrcpp::Any ASTGenerator::visitProcedure(gazprea::GazpreaParser::ProcedureContext *ctx) {
    std::string retType = "void";
    if(ctx->returnStat()) retType = ctx->returnStat()->type()->getText();
    BlockNode *block  = (BlockNode *) (ASTNode *) visit(ctx->block());
    std::vector<ASTNode *> *params = (std::vector<ASTNode *> *) visit(ctx->params());
    ASTNode * p = (ASTNode *) new ProcedureNode(block, params, retType, ctx->Identifier()->getText(), (int)ctx->getStart()->getLine());
    return p;
}

ASTGenerator::ASTGenerator() {
    globalVars = new std::unordered_set<std::string>();
}

/**
 * TODO - this
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitParams(gazprea::GazpreaParser::ParamsContext *ctx) {
    //returns a vector or parameter nodes
    auto *paramVec = new std::vector<ASTNode *>;
    unsigned long i;
    for (i = 0; i < ctx->param().size(); i++){
        bool constant = nullptr == ctx->param().at(i)->VAR();
        paramVec->push_back(new ParamNode(ctx->param().at(i)->type()->getText(),
                ctx->param().at(i)->Identifier()->getText(), constant, (int)ctx->getStart()->getLine()));
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
    int type;
    if(ctx->STD_INPUT())
        type = INSTREAM;
    else
        type = OUTSTREAM;
    return (ASTNode *) new StreamDeclNode(ctx->Identifier()->getText(), type, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitNormalDecl(gazprea::GazpreaParser::NormalDeclContext *ctx) {
    ASTNode *expr = nullptr;
    if(ctx->expr()){
        expr = (ASTNode *) visit(ctx->expr());
    }

    bool constant = (nullptr != ctx->CONST());
    std::string id = ctx->Identifier()->getText();
    std::string ty;
    if (!ctx->type().empty())  ty = ctx->type(0)->getText();

    // if decl is a tuple decl then
    if ((ctx->type().size() == 1) && (ty.substr(0, 5) == "tuple")) {

        return (ASTNode *) new TupleDeclNode(expr, constant, id, visit(ctx->type(0)), (int)ctx->getStart()->getLine());

    } else { // else it's a normal decl
        auto *typeVec = new std::vector<std::string>();

        unsigned int i;
        for (i = 0; i < ctx->type().size(); i++) {
            typeVec->push_back(ctx->type().at(i)->getText());
        }
        return (ASTNode *) new DeclNode(expr, constant, id, typeVec, expr->getType(), (int)ctx->getStart()->getLine());
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

antlrcpp::Any ASTGenerator::visitProcedureCallDecl(gazprea::GazpreaParser::ProcedureCallDeclContext *ctx) {
    std::string id = ctx->Identifier(0)->getText();
    std::string ty;
    if (!ctx->type().empty())  ty = ctx->type(0)->getText();
    std::string procedureName = ctx->Identifier(1)->getText();
    bool constant = (nullptr != ctx->CONST());

    auto *exprNodes = new std::vector<ASTNode*>;

    for(unsigned int i = 0; i < ctx->expr().size(); ++i) {
        ASTNode * node = (ASTNode *) visit(ctx->expr()[i]);
        exprNodes->push_back(node);
    }

    auto *typeVec = new std::vector<std::string>();
    unsigned int i;
    for (i = 0; i < ctx->type().size(); i++) {
        typeVec->push_back(ctx->type().at(i)->getText());
    }

    int operation = PLUS;
    if (ctx->op) {
        if (ctx->op->getType() == gazprea::GazpreaParser::NOT) {
            operation = NEG;
        } else if (ctx->op->getType() == gazprea::GazpreaParser::ADD) {
            operation = PLUS;
        } else if (ctx->op->getType() == gazprea::GazpreaParser::SUB) {
            operation = MINUS;
        }
    }

    return (ASTNode *) new ProcedureCallNode(id,procedureName, exprNodes, typeVec, constant, operation, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitProcedureCallAss(gazprea::GazpreaParser::ProcedureCallAssContext *ctx) {
    std::string id = ctx->Identifier(0)->getText();
    std::string procedureName = ctx->Identifier(1)->getText();

    auto *exprNodes = new std::vector<ASTNode*>;

    for(unsigned int i = 0; i < ctx->expr().size(); ++i) {
        ASTNode * node = (ASTNode *) visit(ctx->expr()[i]);
        exprNodes->push_back(node);
    }

    int operation = PLUS;
    if (ctx->op) {
        if (ctx->op->getType() == gazprea::GazpreaParser::NOT) {
            operation = NEG;
        } else if (ctx->op->getType() == gazprea::GazpreaParser::ADD) {
            operation = PLUS;
        } else if (ctx->op->getType() == gazprea::GazpreaParser::SUB) {
            operation = MINUS;
        }
    }

    return (ASTNode *) new ProcedureCallNode(id,procedureName, exprNodes, operation, (int)ctx->getStart()->getLine());
}

/*
 * ignore the warning on dynamic cast
 */
antlrcpp::Any ASTGenerator::visitTupleIndexExpr(gazprea::GazpreaParser::TupleIndexExprContext *ctx) {
    std::string idName = ctx->tupleMember()->TupleIndex()->getText();
    idName.erase(std::remove(idName.begin(), idName.end(), '.'), idName.end());
    auto idNode = (ASTNode *) new IDNode(idName, (int)ctx->getStart()->getLine());
    ASTNode *index;
    if(ctx->tupleMember()->Integer()){
        int val = std::stoi(ctx->tupleMember()->Integer()->getText());
        assert(val > 0);
        --val;
        index = (ASTNode *) new INTNode(val, (int)ctx->getStart()->getLine());
    }
    else{
        assert(ctx->tupleMember()->Identifier());
        index = (ASTNode *) new IDNode(ctx->tupleMember()->Identifier()->getText(), (int)ctx->getStart()->getLine());
    }
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
    unsigned long int i;
    for(i = 0; i < ctx->expr().size(); i++){
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
    if ((ctx->type().size() == 1) && (ty.substr(0, 5) == "tuple")) {
        return (ASTNode *) new TupleDeclNode(expr, constant, id, visit(ctx->type(0)), (int)ctx->getStart()->getLine());

    }
    else { // else it's a normal decl
        auto *typeVec = new std::vector<std::string>();

        unsigned int i;
        for (i = 0; i < ctx->type().size(); i++) {
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

antlrcpp::Any ASTGenerator::visitProtoFunc(gazprea::GazpreaParser::ProtoFuncContext *ctx) {
    std::string retType = "void";
    if(ctx->returnStat()) retType = ctx->returnStat()->type()->getText();
    std::vector<ASTNode *> *params = (std::vector<ASTNode *> *) visit(ctx->params());
    ASTNode * p = (ASTNode *) new ProtoProcedureNode(params, retType, ctx->Identifier()->getText(), (int)ctx->getStart()->getLine());
    return p;
}

antlrcpp::Any ASTGenerator::visitGlobalDecl(gazprea::GazpreaParser::GlobalDeclContext *ctx) {
    ASTNode *expr  = (ASTNode *) visit(ctx->expr());
    std::string id = ctx->Identifier()->getText();
    auto *typeVec  = new std::vector<std::string>();

    unsigned int i;
    for (i = 0; i < ctx->type().size(); i++) {
        typeVec->push_back(ctx->type().at(i)->getText());
    }
    globalVars->insert(id);
    return (ASTNode *) new GlobalDeclNode(expr, id, typeVec, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitTupleMemberAss(gazprea::GazpreaParser::TupleMemberAssContext *ctx) {
    std::string idName = ctx->tupleMember()->TupleIndex()->getText();
    idName.erase(std::remove(idName.begin(), idName.end(), '.'), idName.end());
    ASTNode *expr  = (ASTNode *) visit(ctx->expr());

    auto idNode =  new IDNode(idName, (int)ctx->getStart()->getLine());
    ASTNode *index;
    if(ctx->tupleMember()->Integer()){
        int val = std::stoi(ctx->tupleMember()->Integer()->getText());
        assert(val > 0);
        --val;
        index = (ASTNode *) new INTNode(val, (int)ctx->getStart()->getLine());
    }
    else{
        assert(ctx->tupleMember()->Identifier());
        index = (ASTNode *) new IDNode(ctx->tupleMember()->Identifier()->getText(), (int)ctx->getStart()->getLine());
    }

    auto LHS = new IndexTupleNode(index, idNode, (int)ctx->getStart()->getLine());

   return (ASTNode *) new TupleMemberAssNode(expr, LHS, (int)ctx->getStart()->getLine());
}
