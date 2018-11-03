//
// Created by kyle on 20/10/18.
//
#include <algorithm>
#include <AST/ASTGenerator.h>
#include <AST/ASTNodes/BaseNodes/BasicBlockNode.h>
#include <AST/ASTNodes/FuncProcNodes/ProcedureNode.h>
#include <AST/ASTNodes/StatementNodes/ReturnNode.h>
#include <AST/ASTNodes/TerminalNodes/INTNode.h>
#include <AST/ASTNodes/TerminalNodes/RealNode.h>
#include <AST/ASTNodes/StatementNodes/DeclNode.h>
#include <AST/ASTNodes/FuncProcNodes/CallNode.h>
#include <AST/ASTNodes/StatementNodes/CastExprNode.h>
#include <AST/ASTNodes/TerminalNodes/TupleNode.h>
#include <AST/ASTNodes/StatementNodes/TupleDeclNode.h>

#include "../include/AST/ASTGenerator.h"

antlrcpp::Any ASTGenerator::visitFile(gazprea::GazpreaParser::FileContext *ctx) {
    auto *procedures = new std::vector<ASTNode *>;
    unsigned long i;
    for (i = 0; i < ctx->procedure().size(); i++){
        //printf("%lu\n", i);
        ASTNode * p = (ASTNode *) visit(ctx->procedure().at(i));
        if (nullptr == p){
            printf("yes\n");
        }
        procedures->push_back(p);
    }
    return (ASTNode *) new FileNode(procedures);
}

antlrcpp::Any ASTGenerator::visitExponentExpr(gazprea::GazpreaParser::ExponentExprContext *ctx) {
    return GazpreaBaseVisitor::visitExponentExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitIntegerExpr(gazprea::GazpreaParser::IntegerExprContext *ctx) {
    std::string rawStr = ctx->getText();
    rawStr.erase(std::remove(rawStr.begin(), rawStr.end(), '_'), rawStr.end());     // remove the underscores
    int val = std::stoi(rawStr);
    return (ASTNode *) new INTNode(val);
}

antlrcpp::Any ASTGenerator::visitNullExpr(gazprea::GazpreaParser::NullExprContext *ctx) {
    return (ASTNode *) new IDNode("null");
}

antlrcpp::Any ASTGenerator::visitCastExpr(gazprea::GazpreaParser::CastExprContext *ctx) {
    ASTNode * expr = (ASTNode *) visit(ctx->expr());

    return (ASTNode *) new CastExprNode(expr, ctx->type()->getText());
}

antlrcpp::Any ASTGenerator::visitRealExpr(gazprea::GazpreaParser::RealExprContext *ctx) {
    std::string strVal = ctx->getText();
    std::string str2Val;

    std::copy_if (strVal.begin(), strVal.end(), std::back_inserter(str2Val), [](char i){return i != '_';} ); // remove _
    float val = std::stof(str2Val);
    return (ASTNode *) new RealNode(val);}

antlrcpp::Any ASTGenerator::visitBrackExpr(gazprea::GazpreaParser::BrackExprContext *ctx) {
    return (ASTNode *) visit(ctx->expr());
}



antlrcpp::Any ASTGenerator::visitLessExpr(gazprea::GazpreaParser::LessExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    if(ctx->op->getType() == gazprea::GazpreaParser::LESST){
        return (ASTNode *) new LTNode(left, right);
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::LESSTE){
        return (ASTNode *) new LTENode(left, right);
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::MORET){
        return (ASTNode *) new GTNode(left, right);
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::MORETE){
        return (ASTNode *) new GTENode(left, right);
    }
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitEqlExpr(gazprea::GazpreaParser::EqlExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    if(ctx->op->getType() == gazprea::GazpreaParser::EEQL){
        return (ASTNode *) new EQNode(left, right);
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::NEQL){
        return (ASTNode *) new NEQNode(left, right);
    }
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitOrExpr(gazprea::GazpreaParser::OrExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    if(ctx->op->getType() == gazprea::GazpreaParser::OR){
        return (ASTNode *) new OrNode(left, right);
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::XOR){
        return (ASTNode *) new XOrNode(left, right);
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
        ASTNode * zero = (ASTNode *) new INTNode(0);
        return (ASTNode *) new SubNode(zero, expr);
    }
    else if(ctx->NOT()){
        return (ASTNode *) new NegateNode(expr);
    }

    return GazpreaBaseVisitor::visitUnaryExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitAddExpr(gazprea::GazpreaParser::AddExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    if(ctx->op->getType() == gazprea::GazpreaParser::ADD) {
        return (ASTNode *) new AddNode(left, right);
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::SUB) {
        return (ASTNode *) new SubNode(left, right);
    }
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitMulExpr(gazprea::GazpreaParser::MulExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    if(ctx->op->getType() == gazprea::GazpreaParser::MUL) {
        return (ASTNode *) new MulNode(left, right);
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::DIV) {
        return (ASTNode *) new DivNode(left, right);
    }
    else if(ctx->op->getType() == gazprea::GazpreaParser::REM) {
        return (ASTNode *) new RemNode(left, right);
    }
    return nullptr;
}


antlrcpp::Any ASTGenerator::visitIdentifierExpr(gazprea::GazpreaParser::IdentifierExprContext *ctx) {
    return (ASTNode *) new IDNode(ctx->Identifier()->getText());
}

antlrcpp::Any ASTGenerator::visitAndExpr(gazprea::GazpreaParser::AndExprContext *ctx) {
    ASTNode * left  = (ASTNode *) visit(ctx->left);
    ASTNode * right = (ASTNode *) visit(ctx->right);
    return (ASTNode *) new AndNode(left, right);
}

antlrcpp::Any ASTGenerator::visitStatement(gazprea::GazpreaParser::StatementContext *ctx) {
    return GazpreaBaseVisitor::visitStatement(ctx);
}

antlrcpp::Any ASTGenerator::visitNormalAss(gazprea::GazpreaParser::NormalAssContext *ctx) {
    ASTNode *expr = nullptr;
    if(ctx->expr())
        expr = (ASTNode *) visit(ctx->expr());
    return (ASTNode *) new AssignNode(expr, ctx->Identifier()->getText());
}



antlrcpp::Any ASTGenerator::visitConditional(gazprea::GazpreaParser::ConditionalContext *ctx) {
    std::vector<ASTNode *> *conds  = new std::vector<ASTNode *>;
    std::vector<ASTNode *> *blocks = new std::vector<ASTNode *>;

    unsigned long int i;
    for(i = 0; i < ctx->expr().size(); i++){
        conds->push_back((ASTNode *) visit(ctx->expr().at(i)));
    }
    for(i = 0; i < ctx->block().size(); i++){
        blocks->push_back((ASTNode *) visit(ctx->block().at(i)));
    }

    return (ASTNode *) new CondNode(conds, blocks);
}

antlrcpp::Any ASTGenerator::visitInfiniteLoop(gazprea::GazpreaParser::InfiniteLoopContext *ctx) {
    return (ASTNode *) new LoopNode((ASTNode *) visit(ctx->block()));
}

antlrcpp::Any ASTGenerator::visitPredicatedLoop(gazprea::GazpreaParser::PredicatedLoopContext *ctx) {
    return (ASTNode *) new LoopNode((ASTNode *) visit(ctx->block()), (ASTNode *) visit(ctx->expr()));
}

antlrcpp::Any ASTGenerator::visitDoLoop(gazprea::GazpreaParser::DoLoopContext *ctx) {
    return (ASTNode *) new LoopNode((ASTNode *) visit(ctx->block()), (ASTNode *) visit(ctx->expr()));
}

antlrcpp::Any ASTGenerator::visitIteratorLoop(gazprea::GazpreaParser::IteratorLoopContext *ctx) {
    return GazpreaBaseVisitor::visitIteratorLoop(ctx);
}

antlrcpp::Any ASTGenerator::visitBlock(gazprea::GazpreaParser::BlockContext *ctx) {
    ASTNode *declBlock;
    ASTNode *bodyBlock;

    if(ctx->decBlock())  declBlock = (ASTNode *) visit(ctx->decBlock());
    else declBlock = new BasicBlockNode(new std::vector<ASTNode *>);
    if(ctx->bodyBlock()) bodyBlock = (ASTNode *) visit(ctx->bodyBlock());
    else bodyBlock = new BasicBlockNode(new std::vector<ASTNode *>);

    return (ASTNode *) new BlockNode(declBlock, bodyBlock);
}

antlrcpp::Any ASTGenerator::visitDecBlock(gazprea::GazpreaParser::DecBlockContext *ctx) {
    auto *statements = new std::vector<ASTNode *>;
    unsigned int i;
    for(i=0;i<ctx->declaration().size();i++){
        statements->push_back( (ASTNode *) visit(ctx->declaration()[i]));
    }
    return (ASTNode *) new BasicBlockNode(statements);
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
    }
    return (ASTNode *) new BasicBlockNode(statements);
}

antlrcpp::Any ASTGenerator::visitOutStream(gazprea::GazpreaParser::OutStreamContext *ctx) {
    ASTNode * expr = (ASTNode *) visit(ctx->expr());
    return (ASTNode *) new OutputNode(ctx->Identifier()->getText(), expr);
}

antlrcpp::Any ASTGenerator::visitInStream(gazprea::GazpreaParser::InStreamContext *ctx) {
    return (ASTNode *) new InputNode(ctx->Identifier().at(1)->getText(), ctx->Identifier().at(0)->getText());
}

antlrcpp::Any ASTGenerator::visitTypeDefine(gazprea::GazpreaParser::TypeDefineContext *ctx) {
    return GazpreaBaseVisitor::visitTypeDefine(ctx);
}

antlrcpp::Any ASTGenerator::visitProcedureCall(gazprea::GazpreaParser::ProcedureCallContext *ctx) {
    std::vector<ASTNode*> *exprNodes = new std::vector<ASTNode*>;
    for(unsigned int i = 0; i < ctx->expr().size(); ++i) {
        ASTNode * node = (ASTNode *) visit(ctx->expr()[i]);
        exprNodes->push_back(node);
    }
    return (ASTNode *) new CallNode(exprNodes, ctx->Identifier()->getText());
}

antlrcpp::Any ASTGenerator::visitType(gazprea::GazpreaParser::TypeContext *ctx) {
    return GazpreaBaseVisitor::visitType(ctx);
}

antlrcpp::Any ASTGenerator::visitProcedure(gazprea::GazpreaParser::ProcedureContext *ctx) {
    std::string retType = "void";
    if(ctx->returnStat()) retType = ctx->returnStat()->type()->getText();
    BlockNode *block  = (BlockNode *) (ASTNode *) visit(ctx->block());
    std::vector<ASTNode *> *params = (std::vector<ASTNode *> *) visit(ctx->params());
    ASTNode * p = (ASTNode *) new ProcedureNode(block, params, retType, ctx->Identifier()->getText());
    return p;
}

ASTGenerator::ASTGenerator() {}

/**
 * TODO - this
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitParams(gazprea::GazpreaParser::ParamsContext *ctx) {
    //returns a vector or parameter nodes
    auto *paramVec = new std::vector<ASTNode *>;
    unsigned long i;
    for (i = 0; i < ctx->Identifier().size(); i++){
        ctx->type().at(i);
        ctx->Identifier().at(i);
        paramVec->push_back(new ParamNode(ctx->type().at(i)->getText(), ctx->Identifier().at(i)->getText()));
    }

    return (std::vector<ASTNode *> *) paramVec;
}

antlrcpp::Any ASTGenerator::visitReturnStat(gazprea::GazpreaParser::ReturnStatContext *ctx) {
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitReturnCall(gazprea::GazpreaParser::ReturnCallContext *ctx) {
    ASTNode * expr = nullptr;
    if(ctx->expr())
        expr = (ASTNode *) visit(ctx->expr());
    return (ASTNode *) new ReturnNode(expr);
}


antlrcpp::Any ASTGenerator::visitBoolExpr(gazprea::GazpreaParser::BoolExprContext *ctx) {
    std::string strExpr = ctx->getText();
    if(strExpr == "true") {
        bool val = true;
        return (ASTNode *) new BoolNode(val);
    }
    else if(strExpr == "false") {
        bool val = false;
        return (ASTNode *) new BoolNode(val);
    }
    return nullptr;
}


antlrcpp::Any ASTGenerator::visitStreamDecl(gazprea::GazpreaParser::StreamDeclContext *ctx) {
    int type;
    if(ctx->STD_INPUT())
        type = INSTREAM;
    else
        type = OUTSTREAM;
    return (ASTNode *) new StreamDeclNode(ctx->Identifier()->getText(), type);
}

antlrcpp::Any ASTGenerator::visitNormalDecl(gazprea::GazpreaParser::NormalDeclContext *ctx) {
    ASTNode *expr = nullptr;
    if(ctx->expr()){
        expr = (ASTNode *) visit(ctx->expr());
    }

    bool constant = (nullptr == ctx->CONST());
    std::string id = ctx->Identifier()->getText();
    std::string ty = ctx->type(0)->getText();

    // if decl is a tuple decl then
    if ((ctx->type().size() == 1) && (ty.substr(0, 5) == "tuple")) {
//        auto tupleType = visit(ctx->type(0));         // get the tuple type
        //        return (ASTNode *) new TupleDeclNode(stuff);
        return nullptr;

    } else { // else it's a normal decl
        auto *typeVec = new std::vector<std::string>();

        unsigned int i;
        for (i = 0; i < ctx->type().size(); i++) {
            typeVec->push_back(ctx->type().at(i)->getText());
        }

        return (ASTNode *) new DeclNode(expr, constant, id, typeVec, expr->getType());
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

    return (ASTNode *) new CharNode(val);
}

antlrcpp::Any ASTGenerator::visitProcedureCallDecl(gazprea::GazpreaParser::ProcedureCallDeclContext *ctx) {
    return GazpreaBaseVisitor::visitProcedureCallDecl(ctx);
}

antlrcpp::Any ASTGenerator::visitProcedureCallAss(gazprea::GazpreaParser::ProcedureCallAssContext *ctx) {
    return GazpreaBaseVisitor::visitProcedureCallAss(ctx);
}

antlrcpp::Any ASTGenerator::visitTupleIndexExpr(gazprea::GazpreaParser::TupleIndexExprContext *ctx) {
    return GazpreaBaseVisitor::visitTupleIndexExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitPythonTupleAss(gazprea::GazpreaParser::PythonTupleAssContext *ctx) {
    return GazpreaBaseVisitor::visitPythonTupleAss(ctx);
}

// this just calls visitTuple
antlrcpp::Any ASTGenerator::visitTupleExpr(gazprea::GazpreaParser::TupleExprContext *ctx) {
    return GazpreaBaseVisitor::visitTupleExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitTuple(gazprea::GazpreaParser::TupleContext *ctx) {
    auto *conds  = new std::vector<ASTNode *>;
    unsigned long int i;
    for(i = 0; i < ctx->expr().size(); i++){
        conds->push_back((ASTNode *) visit(ctx->expr().at(i)));
    }

    return (ASTNode *) new TupleNode(conds);
}

antlrcpp::Any ASTGenerator::visitTupleType(gazprea::GazpreaParser::TupleTypeContext *ctx) {
    //


//        auto *typeVec = new std::vector<std::string>();
//        auto *IDVec = new std::vector<std::string>();
//
//
//        return (ASTNode *) new TupleDeclNode(expr, typeVec, IDVec,ID);
   return nullptr;
}

