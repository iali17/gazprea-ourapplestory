//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTGENERATOR_H
#define GAZPREABASE_ASTGENERATOR_H

#include "GazpreaBaseVisitor.h"
#include "AST.h"
class ASTGenerator : public gazprea::GazpreaBaseVisitor {
public:
    ASTGenerator();
private:
    antlrcpp::Any visitFile(gazprea::GazpreaParser::FileContext *ctx) override;
    antlrcpp::Any visitExponentExpr(gazprea::GazpreaParser::ExponentExprContext *ctx) override;
    antlrcpp::Any visitIntegerExpr(gazprea::GazpreaParser::IntegerExprContext *ctx) override;
    antlrcpp::Any visitNullExpr(gazprea::GazpreaParser::NullExprContext *ctx) override;
    antlrcpp::Any visitCastExpr(gazprea::GazpreaParser::CastExprContext *ctx) override;
    antlrcpp::Any visitRealExpr(gazprea::GazpreaParser::RealExprContext *ctx) override;
    antlrcpp::Any visitBrackExpr(gazprea::GazpreaParser::BrackExprContext *ctx) override;
    antlrcpp::Any visitTupleIndexExpr(gazprea::GazpreaParser::TupleIndexExprContext *ctx) override;
    antlrcpp::Any visitLessExpr(gazprea::GazpreaParser::LessExprContext *ctx) override;
    antlrcpp::Any visitEqlExpr(gazprea::GazpreaParser::EqlExprContext *ctx) override;
    antlrcpp::Any visitOrExpr(gazprea::GazpreaParser::OrExprContext *ctx) override;
    antlrcpp::Any visitDomainExpr(gazprea::GazpreaParser::DomainExprContext *ctx) override;
    antlrcpp::Any visitIndexExpr(gazprea::GazpreaParser::IndexExprContext *ctx) override;
    antlrcpp::Any visitUnaryExpr(gazprea::GazpreaParser::UnaryExprContext *ctx) override;
    antlrcpp::Any visitAddExpr(gazprea::GazpreaParser::AddExprContext *ctx) override;
    antlrcpp::Any visitMulExpr(gazprea::GazpreaParser::MulExprContext *ctx) override;
    antlrcpp::Any visitTupleExpr(gazprea::GazpreaParser::TupleExprContext *ctx) override;
    antlrcpp::Any visitIdentifierExpr(gazprea::GazpreaParser::IdentifierExprContext *ctx) override;
    antlrcpp::Any visitAndExpr(gazprea::GazpreaParser::AndExprContext *ctx) override;
    antlrcpp::Any visitStatement(gazprea::GazpreaParser::StatementContext *ctx) override;
    //antlrcpp::Any visitDeclaration(gazprea::GazpreaParser::DeclarationContext *ctx) override;
    antlrcpp::Any visitNormalAss(gazprea::GazpreaParser::NormalAssContext *ctx) override;
    antlrcpp::Any visitTupleAss(gazprea::GazpreaParser::TupleAssContext *ctx) override;
    antlrcpp::Any visitConditional(gazprea::GazpreaParser::ConditionalContext *ctx) override;
    antlrcpp::Any visitInfiniteLoop(gazprea::GazpreaParser::InfiniteLoopContext *ctx) override;
    antlrcpp::Any visitPredicatedLoop(gazprea::GazpreaParser::PredicatedLoopContext *ctx) override;
    antlrcpp::Any visitDoLoop(gazprea::GazpreaParser::DoLoopContext *ctx) override;
    antlrcpp::Any visitIteratorLoop(gazprea::GazpreaParser::IteratorLoopContext *ctx) override;
    antlrcpp::Any visitBlock(gazprea::GazpreaParser::BlockContext *ctx) override;
    antlrcpp::Any visitDecBlock(gazprea::GazpreaParser::DecBlockContext *ctx) override;
    antlrcpp::Any visitBodyBlock(gazprea::GazpreaParser::BodyBlockContext *ctx) override;
    antlrcpp::Any visitOutStream(gazprea::GazpreaParser::OutStreamContext *ctx) override;
    antlrcpp::Any visitInStream(gazprea::GazpreaParser::InStreamContext *ctx) override;
    antlrcpp::Any visitTypeDefine(gazprea::GazpreaParser::TypeDefineContext *ctx) override;
    antlrcpp::Any visitProcedureCall(gazprea::GazpreaParser::ProcedureCallContext *ctx) override;
    antlrcpp::Any visitType(gazprea::GazpreaParser::TypeContext *ctx) override;
    antlrcpp::Any visitProcedure(gazprea::GazpreaParser::ProcedureContext *ctx) override;
    antlrcpp::Any visitParams(gazprea::GazpreaParser::ParamsContext *ctx) override;
    antlrcpp::Any visitReturnStat(gazprea::GazpreaParser::ReturnStatContext *ctx) override;
    antlrcpp::Any visitReturnCall(gazprea::GazpreaParser::ReturnCallContext *ctx) override;

};

#endif //GAZPREABASE_ASTGENERATOR_H
