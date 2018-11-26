//
// Created by ali5 on 11/26/18.
//

#ifndef GAZPREABASE_VECTORERRORNODE_H
#define GAZPREABASE_VECTORERRORNODE_H

#include <string>
#include "ErrorNode.h"

class VectorErrorNode : public ErrorNode {
public:
   VectorErrorNode(std::string &left, std::string &right, int leftSize, int rightSize, int line);
   VectorErrorNode(std::string &left, std::string &right, int vectorSize, bool leftExprSize, int line);

    bool isStv();
    std::string getNode() override;
    std::string getSTVNode();

protected:
    std::string left, right;
    int leftSize, rightSize;
    int leftExprSize;
    int line;

private:
    bool stv;
};

#endif //GAZPREABASE_VECTORERRORNODE_H
