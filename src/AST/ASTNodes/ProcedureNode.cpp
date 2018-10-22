//
// Created by kyle on 22/10/18.
//

#include "AST/ASTNodes/ProcedureNode.h"

ProcedureNode::ProcedureNode(ParamNode *paramNode, BlockNode *blockNode, const std::string &retType,
                             const std::string &procedureName) : paramNode(paramNode), blockNode(blockNode),
                                                                 retType(retType), procedureName(procedureName) {}
