//
// Created by iali on 11/24/18.
//

#include <AST/ASTGenerator.h>

antlrcpp::Any ASTGenerator::visitMatrixType(gazprea::GazpreaParser::MatrixTypeContext *ctx) {
    int line = (int)ctx->getStart()->getLine();
    auto sTypeArray = split(ctx->getText(), '[');
    auto sType = sTypeArray[0];

    if(ctx->extension()) {
        if(ctx->extension()->rightExtension() && ctx->explicitMatrixExtension() == nullptr) {
            return (ASTNode *) new MatrixTypeNode(visit(ctx->extension()), visit(ctx->extension()->rightExtension()), line, sType);
        } else {
            return (ASTNode *) new MatrixTypeNode(visit(ctx->extension()), nullptr, line, sType);
        }
    } else if(ctx->explicitMatrixExtension()) {
        if(ctx->explicitMatrixExtension()->rightExtension()) {
            return (ASTNode *) new MatrixTypeNode(visit(ctx->explicitMatrixExtension()->left), visit(ctx->explicitMatrixExtension()->rightExtension()->right), line, sType);
        } else {
            return (ASTNode *) new MatrixTypeNode(visit(ctx->explicitMatrixExtension()->left), nullptr, line, sType);
        }
    } else {
        //if(ctx->extension()->rightExtension()) {
        //    return (ASTNode *) new MatrixTypeNode(nullptr, visit(ctx->extension()->rightExtension()), line);
        //} else {
            return (ASTNode *) new MatrixTypeNode(nullptr, nullptr, line, sType);
        //}
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
