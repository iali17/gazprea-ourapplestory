//
// Created by iali on 11/24/18.
//

#include <AST/ASTGenerator.h>

antlrcpp::Any ASTGenerator::visitVectorType(gazprea::GazpreaParser::VectorTypeContext *ctx) {
    if (ctx->extension() == nullptr) {
        if (ctx->getText().at(0) == 'b')
            return (ASTNode *) new VectorTypeNode(nullptr, "booleanvector", (int)ctx->getStart()->getLine());
        else if (ctx->getText().at(0) == 'i')
            return (ASTNode *) new VectorTypeNode(nullptr, "integervector", (int)ctx->getStart()->getLine());
        else if (ctx->getText().at(0) == 'c')
            return (ASTNode *) new VectorTypeNode(nullptr, "charactervector", (int)ctx->getStart()->getLine());
        else if (ctx->getText().at(0) == 'r')
            return (ASTNode *) new VectorTypeNode(nullptr, "realvector", (int)ctx->getStart()->getLine());

    } else {
        if (ctx->getText().at(0) == 'b')
            return (ASTNode *) new VectorTypeNode(visit(ctx->extension()), "booleanvector", (int)ctx->getStart()->getLine());
        else if (ctx->getText().at(0) == 'i')
            return (ASTNode *) new VectorTypeNode(visit(ctx->extension()), "integervector", (int)ctx->getStart()->getLine());
        else if (ctx->getText().at(0) == 'c')
            return (ASTNode *) new VectorTypeNode(visit(ctx->extension()), "charactervector", (int)ctx->getStart()->getLine());
        else if (ctx->getText().at(0) == 'r')
            return (ASTNode *) new VectorTypeNode(visit(ctx->extension()), "realvector", (int)ctx->getStart()->getLine());
    }
    return nullptr;
}

antlrcpp::Any ASTGenerator::visitVector(gazprea::GazpreaParser::VectorContext *ctx) {
    auto *expr  = new std::vector<ASTNode *>;

    for(auto exprNode : ctx->expr()){
        expr->push_back((ASTNode *) visit(exprNode));
    }

    return (ASTNode *) new VectorNode(expr, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitEmptyVectorExpr(gazprea::GazpreaParser::EmptyVectorExprContext *ctx) {
    return (ASTNode *) new VectorNode(new std::vector<ASTNode *>, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitIndexExpr(gazprea::GazpreaParser::IndexExprContext *ctx) {
    int line = (int) ctx->getStart()->getLine();
    auto expr = new std::vector<ASTNode *>;
    auto LHS = (ASTNode *) visit(ctx->expr(0));
    ASTNode * one = new INTNode(1, line);
    ASTNode * cur = nullptr;

    for (unsigned long i = 1; i < ctx->expr().size(); ++i) {
        cur = (ASTNode *) visit(ctx->expr(i));

        if(dynamic_cast<IntervalNode *>(cur)){
            cur = (ASTNode *) new ByNode(cur, one, line);
        }
        expr->push_back((ASTNode *) new SubNode(cur, one, line));
    }

    return (ASTNode *) new IndexNode(LHS, expr, line);
}

antlrcpp::Any ASTGenerator::visitVectorLength(gazprea::GazpreaParser::VectorLengthContext *ctx) {
    ASTNode * expr = (ASTNode *) visit(ctx->expr());
    return (ASTNode *) new LengthNode(expr, (int) ctx->getStart()->getLine());
}


antlrcpp::Any ASTGenerator::visitIndexAssign(gazprea::GazpreaParser::IndexAssignContext *ctx) {
    int line      = (int) ctx->getStart()->getLine();
    auto expr     = new std::vector<ASTNode *>;
    auto LHS      = (ASTNode *) visit(ctx->expr(0));
    auto assFrom  = (ASTNode *) visit(ctx->expr(ctx->expr().size() - 1));
    ASTNode * one = new INTNode(1, line);
    ASTNode * cur = nullptr;

    for (unsigned long i = 1; i < ctx->expr().size() - 1; ++i) {
        cur = (ASTNode *) visit(ctx->expr(i));

        if(dynamic_cast<IntervalNode *>(cur)){
            cur = (ASTNode *) new ByNode(cur, one, line);
        }
        expr->push_back((ASTNode *) new SubNode(cur, one, line));
    }

    ASTNode * assTo = (ASTNode *) new IndexNode(LHS, expr, line, true);

    return (ASTNode *) new SliceAssignNode(assTo, assFrom, line);
}
