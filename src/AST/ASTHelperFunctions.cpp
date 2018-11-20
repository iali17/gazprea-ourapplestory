//
// Created by iali1 on 11/20/18.
//

#include <AST/ASTGenerator.h>

ASTNode *ASTGenerator::getIndexNode(std::vector<std::string> values, int num, int lineNum) {
    ASTNode *index;
    assert(!values[num].empty());
    if(isdigit(values[num][0])){
        int val = std::stoi(values[num]);
        assert(val > 0);
        --val;
        index = (ASTNode *) new INTNode(val, lineNum);
    }
    else {
        index = (ASTNode *) new IDNode(values[num], lineNum);
    }
    return index;
}

// from https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
std::vector<std::string> ASTGenerator::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

