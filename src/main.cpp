#include "GazpreaLexer.h"
#include "GazpreaParser.h"

#include "ANTLRFileStream.h"
#include "CommonTokenStream.h"
#include "tree/ParseTree.h"
#include "tree/ParseTreeWalker.h"

#include "AST/ASTGenerator.h"
#include "CodeGenerator/CodeGenerator.h"

#include <iostream>
#include <fstream>


int main(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "Missing required argument.\n"
            << "Required arguments: <input file path> <output file path>\n";
        return 1;
    }

    // Open the file then parse and lex it.
    antlr4::ANTLRFileStream afs(argv[1]);
    gazprea::GazpreaLexer lexer(&afs);
    antlr4::CommonTokenStream tokens(&lexer);
    gazprea::GazpreaParser parser(&tokens);

    // Get the root of the parse tree. Use your base rule name.
    antlr4::tree::ParseTree *tree = parser.file();

    // Make the visitor
    ASTGenerator astGenerator;
    // Visit the tree
    ASTNode *root = (ASTNode *) astGenerator.visit(tree);

    //generate code
    auto codeGenerator = new CodeGenerator(argv[2]);
    codeGenerator->generate(root);

    return 0;
}
