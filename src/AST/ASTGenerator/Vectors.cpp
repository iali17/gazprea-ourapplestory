//
// Created by iali on 11/24/18.
//

#include <AST/ASTGenerator.h>

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

antlrcpp::Any ASTGenerator::visitVectorExpr(gazprea::GazpreaParser::VectorExprContext *ctx) {
    auto *expr  = new std::vector<ASTNode *>;

    for(unsigned long i = 0; i < ctx->vector()->expr().size(); ++i){
        expr->push_back((ASTNode *) visit(ctx->vector()->expr(i)));
    }

    return (ASTNode *) new VectorNode(expr, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitEmptyVectorExpr(gazprea::GazpreaParser::EmptyVectorExprContext *ctx) {
    return (ASTNode *) new VectorNode(new std::vector<ASTNode *>, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitIndexExpr(gazprea::GazpreaParser::IndexExprContext *ctx) {
    auto expr  = new std::vector<ASTNode *>;
    auto LHS = (ASTNode *) visit(ctx->expr(0));

    for(unsigned long i = 1; i < ctx->expr().size(); ++i){
        expr->push_back((ASTNode *) visit(ctx->expr(i)));
    }

    return (ASTNode *) new IndexNode(LHS,expr, (int)ctx->getStart()->getLine());
antlrcpp::Any ASTGenerator::visitVectorLength(gazprea::GazpreaParser::VectorLengthContext *ctx) {
    ASTNode * expr = (ASTNode *) visit(ctx->expr());
    return (ASTNode *) new LengthNode(expr, (int) ctx->getStart()->getLine());
}