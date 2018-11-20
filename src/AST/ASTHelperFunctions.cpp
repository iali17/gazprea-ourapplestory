//
// Created by iali1 on 11/20/18.
//

#include <AST/ASTGenerator.h>

ASTNode *ASTGenerator::getTupleMemberNode(std::vector<std::string> values, int lineNum) {

    ASTNode *index;
    assert(!values[1].empty());
    if(isdigit(values[1][0])){
        int val = std::stoi(values[1]);
        assert(val > 0);
        --val;
        index = (ASTNode *) new INTNode(val, lineNum);
    }
    else {
        index = (ASTNode *) new IDNode(values[1], lineNum);
    }
    return index;
}
