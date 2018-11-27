//
// Created by iali on 11/24/18.
//

#include <AST/ASTGenerator.h>

antlrcpp::Any ASTGenerator::visitStreamDecl(gazprea::GazpreaParser::StreamDeclContext *ctx) {
    assert(not(inFunction));
    int type;
    if(ctx->STD_INPUT())
        type = INSTREAM;
    else
        type = OUTSTREAM;
    std::string id = normalizeID(ctx->Identifier()->getText());
    return (ASTNode *) new StreamDeclNode(id, type, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitNormalDecl(gazprea::GazpreaParser::NormalDeclContext *ctx) {
    int line = (int)ctx->getStart()->getLine();
    ASTNode *expr = nullptr;
    if(ctx->expr()){
        expr = (ASTNode *) visit(ctx->expr());
    }

    bool constant = (nullptr != ctx->CONST());
    std::string id = normalizeID(ctx->Identifier()->getText());
    std::string ty;
    if (!ctx->type().empty()) ty = ctx->type(0)->getText();

    // todo string decl, interval
    // TODO if user defined a type called xxxvector then it would go to vector, same for interval and matrix
    // if decl is a tuple decl then
    if ((ctx->type().size() == 1) && (ty.substr(0, 6) == "tuple(")) {
        return (ASTNode *) new TupleDeclNode(expr, constant, id, visit(ctx->type(0)), line);
    }
    // if matrix decl then
    else if ((ctx->type().size() == 1) && (ctx->type(0)->matrixType())) {
        if (ctx->extension() == nullptr) {  // if extension not provided
            return (ASTNode *) new MatrixDeclNode(expr, line, constant, id, ty, nullptr);
        }
        return (ASTNode *) new MatrixDeclNode(expr, line, constant, id, ty, visit(ctx->extension()));
    }
    // TODO: Parse vector type and add it as a string to vectorDeclNode
    // if vector decl then
    else if ((ctx->type().size() == 1 && ctx->type(0)->vectorType())
    || (ctx->type().size() == 1 && !ctx->type(0)->vectorType() && !ctx->type(0)->matrixType() && ctx->extension())) {
        ASTNode *typeNode;
        if (ctx->type(0)->vectorType()) {
            typeNode = (ASTNode *) visit(ctx->type(0)->vectorType());
        } else {
            typeNode = (ASTNode *) new VectorType(nullptr, ctx->type(0)->getText() + "vector", line);
        }

        ASTNode *size = nullptr;
        if (ctx->extension()){
            size = visit(ctx->extension());
        }
        return (ASTNode *) new VectorDeclNode(expr, constant, id, typeNode, size, line);
    }
    // if interval decl then
    else if (ctx->type().size() == 1  && (ctx->type(0)->intervalType())) {
        return (ASTNode *) new IntervalDeclNode(expr, constant, id, line);
    }
    else { // else it's a normal decl
        auto *typeVec = new std::vector<std::string>();

        for (unsigned long i = 0; i < ctx->type().size(); ++i) {
            typeVec->push_back(ctx->type().at(i)->getText());
        }
        return (ASTNode *) new DeclNode(expr, constant, id, typeVec, expr->getType(), line);
    }
}

antlrcpp::Any ASTGenerator::visitEmptyDecl(gazprea::GazpreaParser::EmptyDeclContext *ctx) {
    int line = (int)ctx->getStart()->getLine();
    auto *expr = (ASTNode *) new NullNode(line);

    bool constant = (nullptr != ctx->CONST());
    std::string id = normalizeID(ctx->Identifier()->getText());
    std::string ty;
    if (!ctx->type().empty())  ty = ctx->type(0)->getText();

    // if decl is a tuple decl then
    if ((ctx->type().size() == 1) && (ty.substr(0, 6) == "tuple(")) {
        return (ASTNode *) new TupleDeclNode(expr, constant, id, visit(ctx->type(0)), line);
    }
    // if decl is a vector type
    else if(ctx->type().size() == 1 && ctx->type(0)->vectorType()) {
        ASTNode *typeNode = (ASTNode *) visit(ctx->type(0)->vectorType());
        ASTNode *size = nullptr;
        if (!ctx->extension()->isEmpty()) {
            size = visit(ctx->extension());
        }
        return (ASTNode *) new VectorDeclNode(expr, constant, id, typeNode, size, (int)ctx->getStart()->getLine());
    }
    // if decl is interval
    else if (ctx->type().size() == 1 && (ctx->type(0)->intervalType())) {
        return (ASTNode *) new IntervalDeclNode(expr, constant, id, line);
    }
    else { // else it's a normal decl
        auto *typeVec = new std::vector<std::string>();
        for (unsigned long i = 0; i < ctx->type().size(); ++i) {
            typeVec->push_back(ctx->type().at(i)->getText());
        }

        return (ASTNode *) new DeclNode(expr, constant, id, typeVec, expr->getType(), line);
    }
}

antlrcpp::Any ASTGenerator::visitGlobalDecl(gazprea::GazpreaParser::GlobalDeclContext *ctx) {
    ASTNode *expr  = (ASTNode *) visit(ctx->expr());
    std::string id = normalizeID(ctx->Identifier()->getText());
    auto *typeVec  = new std::vector<std::string>();

    for (unsigned long i = 0; i < ctx->type().size(); ++i) {
        typeVec->push_back(ctx->type().at(i)->getText());
    }
    globalVars->insert(id);
    return (ASTNode *) new GlobalDeclNode(expr, id, typeVec, (int)ctx->getStart()->getLine());
}

/**
 * Normal assignment
 *  - assigns left to right
 * @param ctx
 * @return
 */
antlrcpp::Any ASTGenerator::visitNormalAss(gazprea::GazpreaParser::NormalAssContext *ctx) {
    ASTNode *expr = nullptr;
    if(ctx->expr())
        expr = (ASTNode *) visit(ctx->expr());
    std::string id = normalizeID(ctx->Identifier()->getText());
    return (ASTNode *) new AssignNode(expr, id, (int)ctx->getStart()->getLine());
}

antlrcpp::Any ASTGenerator::visitStreamAss(gazprea::GazpreaParser::StreamAssContext *ctx) {
    ASTNode * right;
    if(ctx->STD_INPUT()){
        right = (ASTNode *) new IDNode("std_input()", (int)ctx->getStart()->getLine());
    } else {
        right = (ASTNode *) new IDNode("std_output()", (int)ctx->getStart()->getLine());
    }

    std::string id = normalizeID(ctx->Identifier()->getText());
    return (ASTNode *) new AssignNode(right, id, (int)ctx->getStart()->getLine());
}

