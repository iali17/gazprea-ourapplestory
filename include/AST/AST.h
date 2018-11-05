//
// Created by kyle on 20/10/18.
//
// INCLUDES DEFINITIONS OF ALL NODES
//

#ifndef GAZPREABASE_AST_H
#define GAZPREABASE_AST_H

#include "AST/ASTNodes/BaseNodes/ASTNode.h"
#include "AST/ASTNodes/BaseNodes/InfixNode.h"
#include "AST/ASTNodes/BaseNodes/UnaryNode.h"
#include "AST/ASTNodes/BaseNodes/BlockNode.h"
#include "AST/ASTNodes/BaseNodes/BaseBlockParent.h"
#include "AST/ASTNodes/FileNode.h"
#include "AST/ASTNodes/FuncProcNodes/ProcedureNode.h"
#include "AST/ASTNodes/StatementNodes/ReturnNode.h"
#include "AST/ASTNodes/StatementNodes/CondNode.h"
#include "AST/ASTNodes/StatementNodes/DeclNode.h"
#include "AST/ASTNodes/StatementNodes/AssignNode.h"
#include "AST/ASTNodes/StatementNodes/BreakNode.h"
#include "AST/ASTNodes/StatementNodes/ContinueNode.h"
#include "AST/ASTNodes/StatementNodes/CastExprNode.h"
#include "AST/ASTNodes/StatementNodes/LoopNodes/LoopNode.h"
#include "AST/ASTNodes/StatementNodes/LoopNodes/DoLoopNode.h"
#include "AST/ASTNodes/StatementNodes/LoopNodes/InLoopNode.h"
#include "AST/ASTNodes/FuncProcNodes/ParamNode.h"
#include "AST/ASTNodes/TerminalNodes/INTNode.h"
#include "ASTNodes/TerminalNodes/RealNode.h"
#include "ASTNodes/TerminalNodes/CharNode.h"
#include "ASTNodes/TerminalNodes/BoolNode.h"
#include "ASTNodes/TerminalNodes/IDNode.h"
#include "ASTNodes/TerminalNodes/InputNode.h"
#include "ASTNodes/TerminalNodes/OutputNode.h"
#include "ASTNodes/TerminalNodes/StreamDecl.h"
#include "AST/ASTNodes/ArithNodes/AddNode.h"
#include "AST/ASTNodes/ArithNodes/DivNode.h"
#include "AST/ASTNodes/ArithNodes/MulNode.h"
#include "AST/ASTNodes/ArithNodes/SubNode.h"
#include "AST/ASTNodes/ArithNodes/RemNode.h"
#include "AST/ASTNodes/ArithNodes/ExpNode.h"
#include "AST/ASTNodes/BaseNodes/ArithNode.h"
#include "AST/ASTNodes/BaseNodes/CompNode.h"
#include "AST/ASTNodes/BaseNodes/BitArithNode.h"
#include "AST/ASTNodes/CompNodes/EQNode.h"
#include "AST/ASTNodes/CompNodes/NEQNode.h"
#include "AST/ASTNodes/CompNodes/GTNode.h"
#include "AST/ASTNodes/CompNodes/LTNode.h"
#include "AST/ASTNodes/CompNodes/GTENode.h"
#include "AST/ASTNodes/CompNodes/LTENode.h"
#include "AST/ASTNodes/CompNodes/NegateNode.h"
#include "AST/ASTNodes/ArithNodes/AndNode.h"
#include "AST/ASTNodes/ArithNodes/OrNode.h"
#include "AST/ASTNodes/ArithNodes/XOrNode.h"
#include "AST/ASTNodes/FuncProcNodes/ProtoProcedureNode.h"

#endif //GAZPREABASE_AST_H
