//
// Created by iali on 11/24/18.
//

#include <AST/ASTGenerator.h>

antlrcpp::Any ASTGenerator::visitIntervalExpr(gazprea::GazpreaParser::IntervalExprContext *ctx) {
    int line = (int) ctx->getStart()->getLine();
    ASTNode *leftExpr;
    ASTNode *rightExpr = (ASTNode *) visit(ctx->right);

    if(ctx->IntervalThing()) {
        std::string IntervalText = ctx->IntervalThing()->getText();
        std::vector<std::string> values = split(IntervalText,'.');
        std::string idName = values[0];

        for (char letter : idName) {        // do stuff depending on if it's identifier o integer
            if (letter == '_') { continue;}
            else if (isdigit(letter)) {
                idName.erase(std::remove(idName.begin(), idName.end(), '_'), idName.end());     // remove the underscores
                leftExpr = new INTNode(std::stoi(idName), line);
                break;
            }
            else {
                idName = normalizeID(idName);
                leftExpr = new IDNode(idName, line);
                break;
            }
        }
    } else
        leftExpr = (ASTNode *) visit(ctx->left);

    return (ASTNode *) new IntervalNode(leftExpr, rightExpr, line);
}

antlrcpp::Any ASTGenerator::visitByExpr(gazprea::GazpreaParser::ByExprContext *ctx) {
    return (ASTNode *) new ByNode(visit(ctx->left), visit((ctx->right)), (int)ctx->getStart()->getLine());
}

