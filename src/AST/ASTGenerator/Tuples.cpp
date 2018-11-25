//
// Created by iali on 11/24/18.
//

#include <AST/ASTGenerator.h>

antlrcpp::Any ASTGenerator::visitTupleMemberAss(gazprea::GazpreaParser::TupleMemberAssContext *ctx) {
    std::string tupleText = ctx->TupleIndex()->getText();
    std::vector<std::string> values = split(tupleText,'.');
    std::string idName = values[0];
    ASTNode *expr  = (ASTNode *) visit(ctx->expr());

    int lineNum = (int)ctx->getStart()->getLine();
    auto idNode =  (ASTNode*) new IDNode(idName, lineNum);
    ASTNode *index = getIndexNode(values, 1, lineNum);

    auto LHS = new IndexTupleNode(index, dynamic_cast<IDNode*>(idNode), (int)ctx->getStart()->getLine());

    return (ASTNode *) new TupleMemberAssNode(expr, LHS, (int)ctx->getStart()->getLine());
}

// everything inside the tuple() is a empty decl
antlrcpp::Any ASTGenerator::visitTupleTypeIdentifier(gazprea::GazpreaParser::TupleTypeIdentifierContext *ctx) {
    ASTNode *expr = (ASTNode *) new IDNode("null", (int)ctx->getStart()->getLine());
    bool constant = false;

    auto *typeVec = new std::vector<std::string>();
    typeVec->push_back(ctx->type()->getText());

    if (ctx->Identifier() == nullptr) {
        return (ASTNode *) new DeclNode(expr, constant, "", typeVec, expr->getType(), (int)ctx->getStart()->getLine());
    } else {
        return (ASTNode *) new DeclNode(expr, constant, ctx->Identifier()->getText(), typeVec, expr->getType(), (int)ctx->getStart()->getLine());
    }
}

antlrcpp::Any ASTGenerator::visitPythonTupleAss(gazprea::GazpreaParser::PythonTupleAssContext *ctx) {
    ASTNode *expr = nullptr;
    if(ctx->expr())
        expr = (ASTNode *) visit(ctx->expr());

    auto *IDs = new std::vector<std::string>;
    for (auto element : ctx->Identifier()) {
        IDs->push_back(element->getText());
    }

    return (ASTNode *) new PythonTupleAssNode(expr, *IDs, (int)ctx->getStart()->getLine());
}

/*
 * ignore the warning on dynamic cast
 */
antlrcpp::Any ASTGenerator::visitTupleIndexExpr(gazprea::GazpreaParser::TupleIndexExprContext *ctx) {
    std::string tupleText = ctx->TupleIndex()->getText();
    std::vector<std::string> values = split(tupleText,'.');
    std::string idName = values[0];

    int lineNum = (int)ctx->getStart()->getLine();
    auto idNode = (ASTNode *) new IDNode(idName, lineNum);
    ASTNode *index = getIndexNode(values, 1, lineNum);

    return (ASTNode *) new IndexTupleNode(index, dynamic_cast<IDNode*>(idNode), (int)ctx->getStart()->getLine());
}

// this just calls visitTuple
antlrcpp::Any ASTGenerator::visitTupleExpr(gazprea::GazpreaParser::TupleExprContext *ctx) {
    return (ASTNode *) GazpreaBaseVisitor::visitTupleExpr(ctx);
}

antlrcpp::Any ASTGenerator::visitTuple(gazprea::GazpreaParser::TupleContext *ctx) {
    auto *expr  = new std::vector<ASTNode *>;
    for(unsigned long i = 0; i < ctx->expr().size(); ++i){
        expr->push_back((ASTNode *) visit(ctx->expr(i)));
    }
    return (ASTNode *) new TupleNode(expr, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitTupleType(gazprea::GazpreaParser::TupleTypeContext *ctx) {
    auto *typeIdNodes = new std::vector<ASTNode *>;

    for (auto typeId : ctx->tupleTypeIdentifier()) {
        typeIdNodes->push_back((ASTNode *) visit(typeId));
    }

    return (ASTNode *) new TupleType(typeIdNodes, (int)ctx->getStart()->getLine());
}

