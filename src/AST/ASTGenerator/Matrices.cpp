//
// Created by iali on 11/24/18.
//

#include <AST/ASTGenerator.h>

antlrcpp::Any ASTGenerator::visitMatrixType(gazprea::GazpreaParser::MatrixTypeContext *ctx) {
    return nullptr;
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
