//
// Created by iali on 11/24/18.
//

#include <AST/ASTGenerator.h>

antlrcpp::Any ASTGenerator::visitMatrixType(gazprea::GazpreaParser::MatrixTypeContext *ctx) {
    int line = (int)ctx->getStart()->getLine();

    if(ctx->extension()) {
        if(ctx->extension()->rightExtension()) {
            return (ASTNode *) new MatrixType(visit(ctx->extension()), visit(ctx->extension()->rightExtension()), line);
        } else {
            return (ASTNode *) new MatrixType(visit(ctx->extension()), nullptr, line);
        }
    } else {
        if(ctx->extension()->rightExtension()) {
            return (ASTNode *) new MatrixType(nullptr, visit(ctx->extension()->rightExtension()), line);
        } else {
            return (ASTNode *) new MatrixType(nullptr, nullptr, line);
        }
    }
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
