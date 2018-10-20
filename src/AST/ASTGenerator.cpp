//
// Created by kyle on 20/10/18.
//

#include <AST/ASTGenerator.h>

#include "../include/AST/ASTGenerator.h"

antlrcpp::Any ASTGenerator::visitFile(gazprea::GazpreaParser::FileContext *ctx) {
    return GazpreaBaseVisitor::visitFile(ctx);
}

antlrcpp::Any ASTGenerator::visitExponentExpr(gazprea::GazpreaParser::ExponentExprContext *ctx) {
    return GazpreaBaseVisitor::visitExponentExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitIntegerExpr(gazprea::GazpreaParser::IntegerExprContext *ctx) {
    return GazpreaBaseVisitor::visitIntegerExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitNullExpr(gazprea::GazpreaParser::NullExprContext *ctx) {
    return GazpreaBaseVisitor::visitNullExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitCastExpr(gazprea::GazpreaParser::CastExprContext *ctx) {
    return GazpreaBaseVisitor::visitCastExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitRealExpr(gazprea::GazpreaParser::RealExprContext *ctx) {
    return GazpreaBaseVisitor::visitRealExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitBrackExpr(gazprea::GazpreaParser::BrackExprContext *ctx) {
    return GazpreaBaseVisitor::visitBrackExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitTupleIndexExpr(gazprea::GazpreaParser::TupleIndexExprContext *ctx) {
    return GazpreaBaseVisitor::visitTupleIndexExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitLessExpr(gazprea::GazpreaParser::LessExprContext *ctx) {
    return GazpreaBaseVisitor::visitLessExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitEqlExpr(gazprea::GazpreaParser::EqlExprContext *ctx) {
    return GazpreaBaseVisitor::visitEqlExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitOrExpr(gazprea::GazpreaParser::OrExprContext *ctx) {
    return GazpreaBaseVisitor::visitOrExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitDomainExpr(gazprea::GazpreaParser::DomainExprContext *ctx) {
    return GazpreaBaseVisitor::visitDomainExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitIndexExpr(gazprea::GazpreaParser::IndexExprContext *ctx) {
    return GazpreaBaseVisitor::visitIndexExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitUnaryExpr(gazprea::GazpreaParser::UnaryExprContext *ctx) {
    return GazpreaBaseVisitor::visitUnaryExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitAddExpr(gazprea::GazpreaParser::AddExprContext *ctx) {
    return GazpreaBaseVisitor::visitAddExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitMulExpr(gazprea::GazpreaParser::MulExprContext *ctx) {
    return GazpreaBaseVisitor::visitMulExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitTupleExpr(gazprea::GazpreaParser::TupleExprContext *ctx) {
    return GazpreaBaseVisitor::visitTupleExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitIdentifierExpr(gazprea::GazpreaParser::IdentifierExprContext *ctx) {
    return GazpreaBaseVisitor::visitIdentifierExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitAndExpr(gazprea::GazpreaParser::AndExprContext *ctx) {
    return GazpreaBaseVisitor::visitAndExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitStatement(gazprea::GazpreaParser::StatementContext *ctx) {
    return GazpreaBaseVisitor::visitStatement(ctx);
}

antlrcpp::Any ASTGenerator::visitDeclaration(gazprea::GazpreaParser::DeclarationContext *ctx) {
    return GazpreaBaseVisitor::visitDeclaration(ctx);
}

antlrcpp::Any ASTGenerator::visitNormalAss(gazprea::GazpreaParser::NormalAssContext *ctx) {
    return GazpreaBaseVisitor::visitNormalAss(ctx);
}

antlrcpp::Any ASTGenerator::visitTupleAss(gazprea::GazpreaParser::TupleAssContext *ctx) {
    return GazpreaBaseVisitor::visitTupleAss(ctx);
}

antlrcpp::Any ASTGenerator::visitConditional(gazprea::GazpreaParser::ConditionalContext *ctx) {
    return GazpreaBaseVisitor::visitConditional(ctx);
}

antlrcpp::Any ASTGenerator::visitInfiniteLoop(gazprea::GazpreaParser::InfiniteLoopContext *ctx) {
    return GazpreaBaseVisitor::visitInfiniteLoop(ctx);
}

antlrcpp::Any ASTGenerator::visitPredicatedLoop(gazprea::GazpreaParser::PredicatedLoopContext *ctx) {
    return GazpreaBaseVisitor::visitPredicatedLoop(ctx);
}

antlrcpp::Any ASTGenerator::visitDoLoop(gazprea::GazpreaParser::DoLoopContext *ctx) {
    return GazpreaBaseVisitor::visitDoLoop(ctx);
}

antlrcpp::Any ASTGenerator::visitIteratorLoop(gazprea::GazpreaParser::IteratorLoopContext *ctx) {
    return GazpreaBaseVisitor::visitIteratorLoop(ctx);
}

antlrcpp::Any ASTGenerator::visitBlock(gazprea::GazpreaParser::BlockContext *ctx) {
    return GazpreaBaseVisitor::visitBlock(ctx);
}

antlrcpp::Any ASTGenerator::visitDecBlock(gazprea::GazpreaParser::DecBlockContext *ctx) {
    return GazpreaBaseVisitor::visitDecBlock(ctx);
}

antlrcpp::Any ASTGenerator::visitBodyBlock(gazprea::GazpreaParser::BodyBlockContext *ctx) {
    return GazpreaBaseVisitor::visitBodyBlock(ctx);
}

antlrcpp::Any ASTGenerator::visitOutStream(gazprea::GazpreaParser::OutStreamContext *ctx) {
    return GazpreaBaseVisitor::visitOutStream(ctx);
}

antlrcpp::Any ASTGenerator::visitInStream(gazprea::GazpreaParser::InStreamContext *ctx) {
    return GazpreaBaseVisitor::visitInStream(ctx);
}

antlrcpp::Any ASTGenerator::visitTypeDefine(gazprea::GazpreaParser::TypeDefineContext *ctx) {
    return GazpreaBaseVisitor::visitTypeDefine(ctx);
}

antlrcpp::Any ASTGenerator::visitProcedureCall(gazprea::GazpreaParser::ProcedureCallContext *ctx) {
    return GazpreaBaseVisitor::visitProcedureCall(ctx);
}

antlrcpp::Any ASTGenerator::visitType(gazprea::GazpreaParser::TypeContext *ctx) {
    return GazpreaBaseVisitor::visitType(ctx);
}

antlrcpp::Any ASTGenerator::visitProcedure(gazprea::GazpreaParser::ProcedureContext *ctx) {
    return GazpreaBaseVisitor::visitProcedure(ctx);
}

ASTGenerator::ASTGenerator() {}
