//
// Created by kyle on 20/10/18.
//
// INCLUDES DEFINITIONS OF ALL NODES
//

#ifndef GAZPREABASE_AST_H
#define GAZPREABASE_AST_H

#include "AST/ASTNodes/ArithNodes/AddNode.h"
#include "AST/ASTNodes/ArithNodes/AndNode.h"
#include "AST/ASTNodes/ArithNodes/DivNode.h"
#include "AST/ASTNodes/ArithNodes/ExpNode.h"
#include "AST/ASTNodes/ArithNodes/MulNode.h"
#include "AST/ASTNodes/ArithNodes/OrNode.h"
#include "AST/ASTNodes/ArithNodes/RemNode.h"
#include "AST/ASTNodes/ArithNodes/SubNode.h"
#include "AST/ASTNodes/ArithNodes/XOrNode.h"

#include "AST/ASTNodes/BaseNodes/ArithNode.h"
#include "AST/ASTNodes/BaseNodes/ASTNode.h"
#include "AST/ASTNodes/BaseNodes/BaseBlockParent.h"
#include "AST/ASTNodes/BaseNodes/BaseLoopNode.h"
#include "AST/ASTNodes/BaseNodes/BaseBlockParent.h"
#include "AST/ASTNodes/BaseNodes/BitArithNode.h"
#include "AST/ASTNodes/BaseNodes/BlockNode.h"
#include "AST/ASTNodes/BaseNodes/CompNode.h"
#include "AST/ASTNodes/BaseNodes/InfixNode.h"
#include "AST/ASTNodes/BaseNodes/IONode.h"
#include "AST/ASTNodes/BaseNodes/UnaryNode.h"

#include "AST/ASTNodes/CompNodes/EQNode.h"
#include "AST/ASTNodes/CompNodes/GTENode.h"
#include "AST/ASTNodes/CompNodes/GTNode.h"
#include "AST/ASTNodes/CompNodes/LTENode.h"
#include "AST/ASTNodes/CompNodes/LTNode.h"
#include "AST/ASTNodes/CompNodes/NegateNode.h"
#include "AST/ASTNodes/CompNodes/NEQNode.h"

#include "AST/ASTNodes/ErrorNodes/ErrorNode.h"
#include "AST/ASTNodes/ErrorNodes/ScalarNode.h"

#include "AST/ASTNodes/FuncProcNodes/CallNode.h"
#include "AST/ASTNodes/FuncProcNodes/FunctionNode.h"
#include "AST/ASTNodes/FuncProcNodes/ParamNode.h"
#include "AST/ASTNodes/FuncProcNodes/ProcedureNode.h"
#include "AST/ASTNodes/FuncProcNodes/ProtoProcedureNode.h"

#include "AST/ASTNodes/StatementNodes/LoopNodes/DoLoopNode.h"
#include "AST/ASTNodes/StatementNodes/LoopNodes/InLoopNode.h"
#include "AST/ASTNodes/StatementNodes/LoopNodes/LoopNode.h"
#include "AST/ASTNodes/StatementNodes/AssignNode.h"
#include "AST/ASTNodes/StatementNodes/BreakNode.h"
#include "AST/ASTNodes/StatementNodes/CastExprNode.h"
#include "AST/ASTNodes/StatementNodes/CondNode.h"
#include "AST/ASTNodes/StatementNodes/ContinueNode.h"
#include "AST/ASTNodes/StatementNodes/DeclNode.h"
#include "AST/ASTNodes/StatementNodes/GlobalDeclNode.h"
#include "AST/ASTNodes/StatementNodes/PythonTupleAssNode.h"
#include "AST/ASTNodes/StatementNodes/ReturnNode.h"
#include "AST/ASTNodes/StatementNodes/TupleDeclNode.h"
#include "AST/ASTNodes/StatementNodes/TupleMemberAssNode.h"

#include "AST/ASTNodes/TerminalNodes/BoolNode.h"
#include "AST/ASTNodes/TerminalNodes/CharNode.h"
#include "AST/ASTNodes/TerminalNodes/GlobalRefNode.h"
#include "AST/ASTNodes/TerminalNodes/IdnNode.h"
#include "AST/ASTNodes/TerminalNodes/IDNode.h"
#include "AST/ASTNodes/TerminalNodes/IndexTupleNode.h"
#include "AST/ASTNodes/TerminalNodes/InputNode.h"
#include "AST/ASTNodes/TerminalNodes/INTNode.h"
#include "AST/ASTNodes/TerminalNodes/NullNode.h"
#include "AST/ASTNodes/TerminalNodes/OutputNode.h"
#include "AST/ASTNodes/TerminalNodes/RealNode.h"
#include "AST/ASTNodes/TerminalNodes/StreamDecl.h"
#include "AST/ASTNodes/TerminalNodes/TupleNode.h"
#include "AST/ASTNodes/TerminalNodes/TupleInputNode.h"
#include "AST/ASTNodes/TerminalNodes/TypeDefNode.h"

#include "AST/ASTNodes/TypeNodes/TupleType.h"

#include "AST/ASTNodes/FileNode.h"

#endif //GAZPREABASE_AST_H
