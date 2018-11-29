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
    std::string id = normalizeID(ctx->Identifier()->getText());
    auto globalVar = globalVars->find(id);

    //check if global
    if (not(globalVar == globalVars->end()))
        return (ASTNode *) new GlobalRefNode(id, (int)ctx->getStart()->getLine());
    auto *idNode = new IDNode(id, (int)ctx->getStart()->getLine());
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
    std::string id = normalizeID(ctx->Identifier()->getText());
    return (ASTNode *) new OutputNode(id, expr, (int)ctx->getStart()->getLine());
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
        std::string idName = normalizeID(values[0]);

        int lineNum = (int)ctx->getStart()->getLine();
        auto idNode =  (ASTNode *) new IDNode(idName, lineNum);
        ASTNode *index = getIndexNode(values, 1,lineNum);

        auto LHS = (ASTNode *) new IndexTupleNode(index, dynamic_cast<IDNode*>(idNode), (int)ctx->getStart()->getLine());
        return (ASTNode *) new TupleInputNode(normalizeID(ctx->Identifier().at(0)->getText()), LHS, (int)ctx->getStart()->getLine());
    }
    return (ASTNode *) new InputNode(normalizeID(ctx->Identifier().at(1)->getText()), normalizeID(ctx->Identifier().at(0)->getText()), (int)ctx->getStart()->getLine());
}

/**
 * TypeDefine
 *
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitTypeDefine(gazprea::GazpreaParser::TypeDefineContext *ctx) {
    std::string type = ctx->type()->getText();
    // Doesnt make sense to normalize these cause it should cause an error
    std::string id = (ctx->Identifier()->getText());

    if(ctx->type()->tupleType()) {
        ASTNode *tuple = (ASTNode *) visit(ctx->type()->tupleType());
        return (ASTNode *) new TypeDefNode(id, tuple, (int)ctx->getStart()->getLine());
    }

    return (ASTNode *) new TypeDefNode(id, type, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitType(gazprea::GazpreaParser::TypeContext *ctx) {
    return GazpreaBaseVisitor::visitType(ctx);
}

ASTGenerator::ASTGenerator() {
    globalVars     = new std::unordered_set<std::string>();
    functionNames  = new std::unordered_set<std::string>();
    procedureNames = new std::unordered_set<std::string>();
    symbolTable    = new SymbolTable();
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

antlrcpp::Any ASTGenerator::visitContinueStat(gazprea::GazpreaParser::ContinueStatContext *ctx) {
    return (ASTNode *) new ContinueNode((int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitBreakStat(gazprea::GazpreaParser::BreakStatContext *ctx) {
    return (ASTNode *) new BreakNode((int)ctx->getStart()->getLine());
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

antlrcpp::Any ASTGenerator::visitExtension(gazprea::GazpreaParser::ExtensionContext *ctx) {

    // if it is a [] of size 2
    if (ctx->rightExtension()) {
        ASTNode *left;

        if (ctx->left == nullptr) {         // being nullptr assumes it's actually a *
            left = nullptr;
        } else {
            if (ctx->left->getText() == "*") {
                return nullptr;
            }
            left = visit(ctx->left);
        }

        return (ASTNode *) new MatrixTypeNode(left, visit(ctx->rightExtension()), (int) ctx->getStart()->getLine());
    }

    // if it is a [] of size 1
    else if (!ctx->rightExtension()) {
        ASTNode *size;

        if(ctx->left == nullptr) {
            size = nullptr;
        } else {
            if (ctx->left->getText() == "*") {
                return nullptr;
            }
            size = visit(ctx->left);
        }

        return size;
    }

    return nullptr;
}

antlrcpp::Any ASTGenerator::visitRightExtension(gazprea::GazpreaParser::RightExtensionContext *ctx) {
    if(ctx->right == nullptr)
        return nullptr;
    else {
        if (ctx->right->getText() == "*") {
            return nullptr;
        }
        return visit(ctx->right);
    }
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
    ASTNode * expr = (ASTNode *) visit(ctx->reverse()->expr());
    return (ASTNode *) new ReverseVectorNode(expr, (int) ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitDotProductExpr(gazprea::GazpreaParser::DotProductExprContext *ctx) {
    ASTNode *left = (ASTNode *) visit(ctx->left);
    ASTNode *right = (ASTNode *) visit(ctx->right);
    return (ASTNode *) new DotProductNode(left, right, (int) ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitConcatExpr(gazprea::GazpreaParser::ConcatExprContext *ctx) {
    ASTNode *left = (ASTNode *) visit(ctx->left);
    ASTNode *right = (ASTNode *) visit(ctx->right);
    return (ASTNode *) new ConcatenationNode(left, right, (int) ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitStreamState(gazprea::GazpreaParser::StreamStateContext *ctx) {
    std::string streamName = normalizeID(ctx->Identifier()->getText());
    return (ASTNode *) new StreamStateNode((int)ctx->getStart()->getLine(),streamName);
}

antlrcpp::Any ASTGenerator::visitGenerator(gazprea::GazpreaParser::GeneratorContext *ctx) {
    auto *loopVars    = new std::vector<std::string>;
    auto *ranges      = new std::vector<ASTNode *>;
    ASTNode *exprNode = nullptr;
    ASTNode *curExpr  = nullptr;
    unsigned int i = 0;

    for(i = 0; i < ctx->Identifier().size(); i++){
        loopVars->push_back(normalizeID(ctx->Identifier().at(i)->getText()));
        curExpr = (ASTNode *) visit(ctx->expr().at(i));
        //youll have to deal with intervals in codegen
        ranges->push_back(curExpr);
    }

    exprNode = (ASTNode *) visit(ctx->expr().at(i));

    return (ASTNode *) new GeneratorNode((int) ctx->getStart()->getLine(), loopVars, ranges, exprNode);
}
