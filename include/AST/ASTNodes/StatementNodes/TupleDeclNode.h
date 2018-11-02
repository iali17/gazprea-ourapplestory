//
// Created by csun on 11/1/18.
//

#ifndef GAZPREABASE_TUPLEDECLNODE_H
#define GAZPREABASE_TUPLEDECLNODE_H


#include <AST/ASTNodes/BaseNodes/UnaryNode.h>

class TupleDeclNode : public UnaryNode {
public:
    TupleDeclNode(ASTNode *expr, std::vector<std::string> *typeID, std::vector<std::string> *identifierID,
                  bool constant, const std::string &ID);


    std::vector<std::string> * typeID;
    std::vector<std::string> * identifierID;
    bool isConstant() const;
    const std::string &getID() const;

protected:
    bool constant;
    std::string ID;



};


#endif //GAZPREABASE_TUPLEDECLNODE_H
