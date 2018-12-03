
//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTGENERATOR_H
#define GAZPREABASE_ASTGENERATOR_H

#include "GazpreaBaseVisitor.h"
#include "AST.h"
#include "Scope/SymbolTable.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

class ASTGenerator : public gazprea::GazpreaBaseVisitor {
public:
    ASTGenerator();
private:
    std::unordered_set<std::string> * globalVars;
    std::unordered_set<std::string> * functionNames;
    std::unordered_set<std::string> * procedureNames;
    SymbolTable                     * symbolTable;
    bool inFunction = false;
    antlrcpp::Any visitFile(gazprea::GazpreaParser::FileContext *ctx) override;
    antlrcpp::Any visitIntegerExpr(gazprea::GazpreaParser::IntegerExprContext *ctx) override;
    antlrcpp::Any visitNullExpr(gazprea::GazpreaParser::NullExprContext *ctx) override;
    antlrcpp::Any visitCastExpr(gazprea::GazpreaParser::CastExprContext *ctx) override;
    antlrcpp::Any visitRealExpr(gazprea::GazpreaParser::RealExprContext *ctx) override;
    antlrcpp::Any visitVector(gazprea::GazpreaParser::VectorContext *ctx) override;
    antlrcpp::Any visitMatrixExpr(gazprea::GazpreaParser::MatrixExprContext *ctx) override;
    antlrcpp::Any visitStringExpr(gazprea::GazpreaParser::StringExprContext *ctx) override;
    antlrcpp::Any visitGeneratorExpr(gazprea::GazpreaParser::GeneratorExprContext *ctx) override;
    antlrcpp::Any visitFilterExpr(gazprea::GazpreaParser::FilterExprContext *ctx) override;
    antlrcpp::Any visitVectorLengthExpr(gazprea::GazpreaParser::VectorLengthExprContext *ctx) override;
    antlrcpp::Any visitRowLengthExpr(gazprea::GazpreaParser::RowLengthExprContext *ctx) override;
    antlrcpp::Any visitColLengthExpr(gazprea::GazpreaParser::ColLengthExprContext *ctx) override;
    antlrcpp::Any visitReverseExpr(gazprea::GazpreaParser::ReverseExprContext *ctx) override;
    antlrcpp::Any visitDotProductOrMatMultExpr(gazprea::GazpreaParser::DotProductOrMatMultExprContext *ctx) override;
    antlrcpp::Any visitByExpr(gazprea::GazpreaParser::ByExprContext *ctx) override;
    antlrcpp::Any visitConcatExpr(gazprea::GazpreaParser::ConcatExprContext *ctx) override;
    antlrcpp::Any visitBrackExpr(gazprea::GazpreaParser::BrackExprContext *ctx) override;
    antlrcpp::Any visitTupleIndexExpr(gazprea::GazpreaParser::TupleIndexExprContext *ctx) override;
    antlrcpp::Any visitIntervalExpr(gazprea::GazpreaParser::IntervalExprContext *ctx) override;
    antlrcpp::Any visitIndexExpr(gazprea::GazpreaParser::IndexExprContext *ctx) override;
    antlrcpp::Any visitUnaryExpr(gazprea::GazpreaParser::UnaryExprContext *ctx) override;
    antlrcpp::Any visitBoolExpr(gazprea::GazpreaParser::BoolExprContext *ctx) override;
    antlrcpp::Any visitTupleExpr(gazprea::GazpreaParser::TupleExprContext *ctx) override;
    antlrcpp::Any visitIdentifierExpr(gazprea::GazpreaParser::IdentifierExprContext *ctx) override;
    antlrcpp::Any visitIdentityExpr(gazprea::GazpreaParser::IdentityExprContext *ctx) override;
    antlrcpp::Any visitStatement(gazprea::GazpreaParser::StatementContext *ctx) override;
    antlrcpp::Any visitNormalAss(gazprea::GazpreaParser::NormalAssContext *ctx) override;
    antlrcpp::Any visitPythonTupleAss(gazprea::GazpreaParser::PythonTupleAssContext *ctx) override;
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
    antlrcpp::Any visitStreamDecl(gazprea::GazpreaParser::StreamDeclContext *ctx) override;
    antlrcpp::Any visitNormalDecl(gazprea::GazpreaParser::NormalDeclContext *ctx) override;
    antlrcpp::Any visitCharExpr(gazprea::GazpreaParser::CharExprContext *ctx) override;
    antlrcpp::Any visitTuple(gazprea::GazpreaParser::TupleContext *ctx) override;
    antlrcpp::Any visitTupleType(gazprea::GazpreaParser::TupleTypeContext *ctx) override;
    antlrcpp::Any visitVectorType(gazprea::GazpreaParser::VectorTypeContext *ctx) override;
    antlrcpp::Any visitMatrixType(gazprea::GazpreaParser::MatrixTypeContext *ctx) override;
    antlrcpp::Any visitEmptyDecl(gazprea::GazpreaParser::EmptyDeclContext *ctx) override;
    antlrcpp::Any visitContinueStat(gazprea::GazpreaParser::ContinueStatContext *ctx) override;
    antlrcpp::Any visitBreakStat(gazprea::GazpreaParser::BreakStatContext *ctx) override;
    antlrcpp::Any visitTupleTypeIdentifier(gazprea::GazpreaParser::TupleTypeIdentifierContext *ctx) override;
    antlrcpp::Any visitStreamAss(gazprea::GazpreaParser::StreamAssContext *ctx) override;
    antlrcpp::Any visitGlobalDecl(gazprea::GazpreaParser::GlobalDeclContext *ctx) override;
    antlrcpp::Any visitTupleMemberAss(gazprea::GazpreaParser::TupleMemberAssContext *ctx) override;
    antlrcpp::Any visitFunction(gazprea::GazpreaParser::FunctionContext *ctx) override;
    antlrcpp::Any visitFunctionReturns(gazprea::GazpreaParser::FunctionReturnsContext *ctx) override;
    antlrcpp::Any visitProcProto(gazprea::GazpreaParser::ProcProtoContext *ctx) override;
    antlrcpp::Any visitFuncProto(gazprea::GazpreaParser::FuncProtoContext *ctx) override;
    antlrcpp::Any visitFunctionCall(gazprea::GazpreaParser::FunctionCallContext *ctx) override;
    antlrcpp::Any visitArithExpr(gazprea::GazpreaParser::ArithExprContext *ctx) override;
    antlrcpp::Any visitOneLineFunction(gazprea::GazpreaParser::OneLineFunctionContext *ctx) override;
    antlrcpp::Any visitCompExpr(gazprea::GazpreaParser::CompExprContext *ctx) override;
    antlrcpp::Any visitMatrix(gazprea::GazpreaParser::MatrixContext *ctx) override;
    antlrcpp::Any visitExtension(gazprea::GazpreaParser::ExtensionContext *ctx) override;
    antlrcpp::Any visitRightExtension(gazprea::GazpreaParser::RightExtensionContext *ctx) override;
    antlrcpp::Any visitStreamState(gazprea::GazpreaParser::StreamStateContext *ctx) override;
    antlrcpp::Any visitGenerator(gazprea::GazpreaParser::GeneratorContext *ctx) override;
    antlrcpp::Any visitEmptyVectorExpr(gazprea::GazpreaParser::EmptyVectorExprContext *ctx) override;
    antlrcpp::Any visitIndexAssign(gazprea::GazpreaParser::IndexAssignContext *ctx) override;
    antlrcpp::Any visitFilter(gazprea::GazpreaParser::FilterContext *ctx) override;
    antlrcpp::Any visitVectorLength(gazprea::GazpreaParser::VectorLengthContext *ctx) override;
    antlrcpp::Any visitFilterIndexExpr(gazprea::GazpreaParser::FilterIndexExprContext *ctx) override;
    antlrcpp::Any visitFullLeftIndexAssign(gazprea::GazpreaParser::FullLeftIndexAssignContext *ctx) override;
    antlrcpp::Any visitFullRightIndexAssign(gazprea::GazpreaParser::FullRightIndexAssignContext *ctx) override;
    antlrcpp::Any visitFullIndexAssign(gazprea::GazpreaParser::FullIndexAssignContext *ctx) override;
    antlrcpp::Any visitFullLeftIndexExpr(gazprea::GazpreaParser::FullLeftIndexExprContext *ctx) override;
    antlrcpp::Any visitFullRightIndexExpr(gazprea::GazpreaParser::FullRightIndexExprContext *ctx) override;
    antlrcpp::Any visitFullIndexExpr(gazprea::GazpreaParser::FullIndexExprContext *ctx) override;
    //Helper Functions
    ASTNode *getIndexNode(std::vector<std::string>values, int num, int lineNum);
    std::vector<std::string> split(const std::string& s, char delimiter);
    std::string normalizeID(std::string oldName);
    BlockNode *convertNodeToBlock(ASTNode * node);
};


#endif //GAZPREABASE_ASTGENERATOR_H

