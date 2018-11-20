//
// Created by iali1 on 11/20/18.
//

#include <AST/ASTGenerator.h>

ASTNode *ASTGenerator::getTupleMemberNode(std::vector<std::string> values, int num, int lineNum) {

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

