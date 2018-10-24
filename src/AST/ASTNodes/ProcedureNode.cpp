//
// Created by kyle on 22/10/18.
//

#include <AST/ASTNodes/ProcedureNode.h>

#include "AST/ASTNodes/ProcedureNode.h"

ProcedureNode::ProcedureNode(ParamNode *paramNode, BlockNode *blockNode, const std::string &retType,
                             const std::string &procedureName) : paramNode(paramNode), blockNode(blockNode),
                                                                 retType(retType), procedureName(procedureName) {
    type = -5;
}

ParamNode *ProcedureNode::getParamNode() const {
    return paramNode;
}

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
