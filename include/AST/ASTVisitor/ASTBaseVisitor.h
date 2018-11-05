//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTBASEVISITOR_H
#define GAZPREABASE_ASTBASEVISITOR_H

#include <AST/ASTNodes/TerminalNodes/INTNode.h>
#include "ASTVisitor.h"
#include <iostream>
#include <AST/ASTNodes/FuncProcNodes/ProcedureCallNode.h>


class ASTBaseVisitor : public ASTVisitor {
public:
    llvm::Value* visit(ASTNode   *node) override {

        if      (dynamic_cast<FileNode *>(node)) {
            return visit((FileNode *) node);
        }
        else if (dynamic_cast<BlockNode *>(node)) {
            return visit(((BlockNode *) node)->getFullBlock());
        }
        else if (dynamic_cast<BasicBlockNode *>(node)) {
            return visit((BasicBlockNode *) node);
        }
        else if (dynamic_cast<ProcedureNode *>(node)) {
            return visit((ProcedureNode *) node);
        }
        else if (dynamic_cast<ParamNode *>(node)) {
            return visit((ParamNode *) node);
        }
        else if (dynamic_cast<ReturnNode *>(node)) {
            return visit((ReturnNode *) node);
        }
        else if (dynamic_cast<INTNode *>(node)) {
            return visit((INTNode *) node);
        }
        else if (dynamic_cast<RealNode *>(node)) {
            return visit((RealNode *) node);
        }
        else if (dynamic_cast<CharNode *>(node)) {
            return visit((CharNode *) node);
        }
        else if (dynamic_cast<BoolNode *>(node)) {
            return visit((BoolNode *) node);
        }
        else if (dynamic_cast<CondNode *>(node)) {
            return visit((CondNode *) node);
        }
        else if (dynamic_cast<LoopNode *>(node)) {
            return visit((LoopNode *) node);
        }
        else if (dynamic_cast<DoLoopNode *>(node)) {
            return visit((DoLoopNode *) node);
        }
        else if (dynamic_cast<InLoopNode *>(node)) {
            return visit((InLoopNode *) node);
        }
        else if (dynamic_cast<AssignNode *>(node)) {
            return visit((AssignNode *) node);
        }
        else if (dynamic_cast<DeclNode *>(node)) {
            return visit((DeclNode *) node);
        }
        else if (dynamic_cast<CastExprNode *>(node)) {
            return visit((CastExprNode *) node);
        }
        else if (dynamic_cast<IDNode *>(node)) {
            return visit((IDNode *) node);
        }
        else if (dynamic_cast<InputNode *>(node)) {
            return visit((InputNode *) node);
        }
        else if (dynamic_cast<OutputNode *>(node)) {
            return visit((OutputNode *) node);
        }
        else if (dynamic_cast<StreamDeclNode *>(node)) {
            return visit((StreamDeclNode *) node);
        }
        else if (dynamic_cast<CallNode *>(node)) {
            return visit((CallNode *) node);
        }
        else if (dynamic_cast<AddNode *>(node)) {
            return visit((AddNode *) node);
        }
        else if (dynamic_cast<SubNode *>(node)) {
            return visit((SubNode *) node);
        }
        else if (dynamic_cast<MulNode *>(node)) {
            return visit((MulNode *) node);
        }
        else if (dynamic_cast<DivNode *>(node)) {
            return visit((DivNode *) node);
        }
        else if (dynamic_cast<RemNode *>(node)) {
            return visit((RemNode *) node);
        }
        else if (dynamic_cast<ExpNode *>(node)) {
            return visit((ExpNode *) node);
        }
        else if (dynamic_cast<EQNode *>(node)) {
            return visit((EQNode *) node);
        }
        else if (dynamic_cast<NEQNode *>(node)) {
            return visit((NEQNode *) node);
        }
        else if (dynamic_cast<LTNode *>(node)) {
            return visit((LTNode *) node);
        }
        else if (dynamic_cast<GTNode *>(node)) {
            return visit((GTNode *) node);
        }
        else if (dynamic_cast<NegateNode *>(node)) {
            return visit((NegateNode *) node);
        }
        else if (dynamic_cast<AndNode *>(node)) {
            return visit((AndNode *) node);
        }
        else if (dynamic_cast<OrNode *>(node)) {
            return visit((OrNode *) node);
        }
        else if (dynamic_cast<XOrNode *>(node)) {
            return visit((XOrNode *) node);
        }
        else if (dynamic_cast<LTENode *>(node)) {
            return visit((LTENode *) node);
        }
        else if (dynamic_cast<GTENode *>(node)) {
            return visit((GTENode *) node);
        }
        else if (dynamic_cast<ContinueNode *>(node)) {
            return visit((ContinueNode *) node);
        }
        else if (dynamic_cast<BreakNode *>(node)) {
            return visit((BreakNode *) node);
        }
        else if (dynamic_cast<ProcedureCallNode *>(node)) {
            return visit((ProcedureCallNode *) node);
        }
        else if (dynamic_cast<TupleDeclNode *>(node)) {
            return visit((TupleDeclNode *) node);
        }
        else if (dynamic_cast<ProtoProcedureNode *>(node)) {
            return visit((ProtoProcedureNode *) node);
        }

        else if (dynamic_cast<TupleType *>(node)) {
            return visit((TupleType *) node);
        }


        return nullptr;
    };

    llvm::Value* visit(FileNode  *node) override { return nullptr;};
    llvm::Value* visit(BasicBlockNode *node) override { return nullptr;};
    llvm::Value* visit(BlockNode *node) override { return nullptr;};
    llvm::Value* visit(ProcedureNode  *node) override { return nullptr;};
    llvm::Value* visit(ParamNode *node) override { return nullptr;};
    llvm::Value* visit(ReturnNode *node) override { return nullptr;};
    llvm::Value* visit(INTNode *node) override { return nullptr;};
    llvm::Value* visit(RealNode *node) override { return nullptr;};
    llvm::Value* visit(CharNode *node) override { return nullptr;};
    llvm::Value* visit(BoolNode *node) override { return nullptr;};
    llvm::Value* visit(CondNode *node) override { return nullptr;};
    llvm::Value* visit(LoopNode *node) override { return nullptr;};
    llvm::Value* visit(InLoopNode *node) override { return nullptr;};
    llvm::Value* visit(DoLoopNode *node) override { return nullptr;};
    llvm::Value* visit(DeclNode *node) override { return nullptr;};
    llvm::Value* visit(AssignNode *node) override { return nullptr;};
    llvm::Value* visit(CastExprNode *node) override { return nullptr;};
    llvm::Value* visit(IDNode *node) override { return nullptr;};
    llvm::Value* visit(InputNode *node) override { return nullptr;};
    llvm::Value* visit(OutputNode *node) override { return nullptr;};
    llvm::Value* visit(StreamDeclNode *node) override { return nullptr;};
    llvm::Value* visit(CallNode *node) override { return nullptr;};
    llvm::Value* visit(AddNode *node) override { return nullptr;};
    llvm::Value* visit(SubNode *node) override { return nullptr;};
    llvm::Value* visit(MulNode *node) override { return nullptr;};
    llvm::Value* visit(DivNode *node) override { return nullptr;};
    llvm::Value* visit(RemNode *node) override { return nullptr;};
    llvm::Value* visit(ExpNode *node) override {return nullptr;};
    llvm::Value* visit(EQNode *node) override { return nullptr;};
    llvm::Value* visit(NEQNode *node) override { return nullptr;};
    llvm::Value* visit(GTNode *node) override { return nullptr;};
    llvm::Value* visit(LTNode *node) override { return nullptr;};
    llvm::Value* visit(AndNode *node) override { return nullptr;};
    llvm::Value* visit(OrNode *node) override { return nullptr;};
    llvm::Value* visit(XOrNode *node) override { return nullptr;};
    llvm::Value* visit(NegateNode *node) override { return nullptr;};
    llvm::Value* visit(GTENode *node) override { return nullptr;};
    llvm::Value* visit(LTENode *node) override { return nullptr;};
    llvm::Value* visit(ContinueNode *node) override { return nullptr;};
    llvm::Value* visit(BreakNode *node) override { return nullptr;};
    llvm::Value* visit(ProcedureCallNode *node) override { return nullptr;};

    llvm::Value* visit(ProtoProcedureNode *node) override { return nullptr;};
    llvm::Value *visit(TupleDeclNode *node) override {return nullptr;}
    llvm::Value *visit(TupleType *node) override { return nullptr; }
};

#endif //GAZPREABASE_ASTBASEVISITOR_H
