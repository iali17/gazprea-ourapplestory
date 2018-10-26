//
// Created by kyle on 22/10/18.
//

#include <AST/ASTNodes/ProcedureNode.h>

#include "AST/ASTNodes/ProcedureNode.h"

BlockNode *ProcedureNode::getBlockNode() const {
    return blockNode;
}

const std::string &ProcedureNode::getRetType() const {
    return retType;
}

const std::string &ProcedureNode::getProcedureName() const {
    return procedureName;
}

BasicBlockNode *ProcedureNode::getFullBlock() {
    BlockNode * block = getBlockNode();
    auto bb1 = (BasicBlockNode *) block->getDeclBlock();
    auto bb2 = (BasicBlockNode *) block->getStateBlock();//new std::vector<ASTNode *>;
    auto full = new std::vector<ASTNode *>;
    full->insert(full->end(), bb1->nodes->begin(), bb1->nodes->end());
    full->insert(full->end(), bb2->nodes->begin(), bb2->nodes->end());
    return new BasicBlockNode(full);
}

ProcedureNode::ProcedureNode(std::vector<ASTNode *> *paramNodes, BlockNode *blockNode, const std::string &retType,
                             const std::string &procedureName) : paramNodes(paramNodes), blockNode(blockNode),
                                                                 retType(retType), procedureName(procedureName) {}

std::vector<ASTNode *> *ProcedureNode::getParamNodes() const {
    return paramNodes;
}
