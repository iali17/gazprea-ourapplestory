//
// Created by kyle on 20/10/18.
//

#ifndef GAZPREABASE_ASTBASEVISITOR_H
#define GAZPREABASE_ASTBASEVISITOR_H
#include "ASTVisitor.h"
#include <iostream>


class ASTBaseVisitor : public ASTVisitor {
public:
    llvm::Value* visit(ASTNode   *node) override {
        llvm::Value *ret = nullptr;
        if      (dynamic_cast<FileNode *>(node)) {
            ret =  visit((FileNode *) node);
        }
        else if (dynamic_cast<BlockNode *>(node)) {
            ret =  visit(((BlockNode *) node)->getFullBlock());
        }
        else if (dynamic_cast<BasicBlockNode *>(node)) {
            ret =  visit((BasicBlockNode *) node);
        }
        else if (dynamic_cast<ProcedureNode *>(node)) {
            ret =  visit((ProcedureNode *) node);
        }
        else if (dynamic_cast<ParamNode *>(node)) {
            ret =  visit((ParamNode *) node);
        }
        else if (dynamic_cast<ReturnNode *>(node)) {
            ret =  visit((ReturnNode *) node);
        }
        else if (dynamic_cast<INTNode *>(node)) {
            ret =  visit((INTNode *) node);
        }
        else if (dynamic_cast<RealNode *>(node)) {
            ret =  visit((RealNode *) node);
        }
        else if (dynamic_cast<CharNode *>(node)) {
            ret =  visit((CharNode *) node);
        }
        else if (dynamic_cast<BoolNode *>(node)) {
            ret =  visit((BoolNode *) node);
        }
        else if (dynamic_cast<NullNode *>(node)) {
            ret =  visit((NullNode *) node);
        }
        else if (dynamic_cast<CondNode *>(node)) {
            ret =  visit((CondNode *) node);
        }
        else if (dynamic_cast<LoopNode *>(node)) {
            ret =  visit((LoopNode *) node);
        }
        else if (dynamic_cast<DoLoopNode *>(node)) {
            ret =  visit((DoLoopNode *) node);
        }
        else if (dynamic_cast<InLoopNode *>(node)) {
            ret =  visit((InLoopNode *) node);
        }
        else if (dynamic_cast<AssignNode *>(node)) {
            ret =  visit((AssignNode *) node);
        }
        else if (dynamic_cast<DeclNode *>(node)) {
            ret =  visit((DeclNode *) node);
        }
        else if (dynamic_cast<CastExprNode *>(node)) {
            ret =  visit((CastExprNode *) node);
        }
        else if (dynamic_cast<IDNode *>(node)) {
            ret =  visit((IDNode *) node);
        }
        else if (dynamic_cast<IdnNode *>(node)) {
            ret =  visit((IdnNode *) node);
        }
        else if (dynamic_cast<InputNode *>(node)) {
            ret =  visit((InputNode *) node);
        }
        else if (dynamic_cast<OutputNode *>(node)) {
            ret =  visit((OutputNode *) node);
        }
        else if (dynamic_cast<StreamDeclNode *>(node)) {
            ret =  visit((StreamDeclNode *) node);
        }
        else if (dynamic_cast<TypeDefNode *>(node)) {
            ret =  visit((TypeDefNode *) node);
        }
        else if (dynamic_cast<CallNode *>(node)) {
            ret =  visit((CallNode *) node);
        }
        else if (dynamic_cast<AddNode *>(node)) {
            ret =  visit((AddNode *) node);
        }
        else if (dynamic_cast<SubNode *>(node)) {
            ret =  visit((SubNode *) node);
        }
        else if (dynamic_cast<MulNode *>(node)) {
            ret =  visit((MulNode *) node);
        }
        else if (dynamic_cast<DivNode *>(node)) {
            ret =  visit((DivNode *) node);
        }
        else if (dynamic_cast<RemNode *>(node)) {
            ret =  visit((RemNode *) node);
        }
        else if (dynamic_cast<ExpNode *>(node)) {
            ret =  visit((ExpNode *) node);
        }
        else if (dynamic_cast<EQNode *>(node)) {
            ret =  visit((EQNode *) node);
        }
        else if (dynamic_cast<NEQNode *>(node)) {
            ret =  visit((NEQNode *) node);
        }
        else if (dynamic_cast<LTNode *>(node)) {
            ret =  visit((LTNode *) node);
        }
        else if (dynamic_cast<GTNode *>(node)) {
            ret =  visit((GTNode *) node);
        }
        else if (dynamic_cast<NegateNode *>(node)) {
            ret =  visit((NegateNode *) node);
        }
        else if (dynamic_cast<AndNode *>(node)) {
            ret =  visit((AndNode *) node);
        }
        else if (dynamic_cast<OrNode *>(node)) {
            ret =  visit((OrNode *) node);
        }
        else if (dynamic_cast<XOrNode *>(node)) {
            ret =  visit((XOrNode *) node);
        }
        else if (dynamic_cast<LTENode *>(node)) {
            ret =  visit((LTENode *) node);
        }
        else if (dynamic_cast<GTENode *>(node)) {
            ret =  visit((GTENode *) node);
        }
        else if (dynamic_cast<ContinueNode *>(node)) {
            ret =  visit((ContinueNode *) node);
        }
        else if (dynamic_cast<BreakNode *>(node)) {
            ret =  visit((BreakNode *) node);
        }
        else if (dynamic_cast<TupleDeclNode *>(node)) {
            ret =  visit((TupleDeclNode *) node);
        }
        else if (dynamic_cast<ProtoProcedureNode *>(node)) {
            ret =  visit((ProtoProcedureNode *) node);
        }
        else if (dynamic_cast<TupleType *>(node)) {
            ret =  visit((TupleType *) node);
        }
        else if (dynamic_cast<VectorType *>(node)) {
            ret = visit((VectorType *) node);
        }
        else if (dynamic_cast<MatrixType *>(node)) {
            ret = visit((MatrixType *) node);
        }
        else if (dynamic_cast<GlobalDeclNode *>(node)) {
            ret =  visit((GlobalDeclNode *) node);
        }
        else if (dynamic_cast<VectorDeclNode *>(node)) {
            ret = visit((VectorDeclNode *) node);
        }
        else if (dynamic_cast<MatrixDeclNode *>(node)) {
            ret = visit((MatrixDeclNode *) node);
        }
        else if (dynamic_cast<GlobalRefNode *>(node)) {
            ret =  visit((GlobalRefNode *) node);
        }
        else if (dynamic_cast<TupleNode *>(node)) {
            ret =  visit((TupleNode *) node);
        }
        else if (dynamic_cast<PythonTupleAssNode *>(node)) {
            ret =  visit((PythonTupleAssNode *) node);
        }
        else if (dynamic_cast<IndexTupleNode *>(node)) {
            ret =  visit((IndexTupleNode *) node);
        }
        else if (dynamic_cast<TupleMemberAssNode *>(node)) {
            ret =  visit((TupleMemberAssNode *) node);
        }
        else if (dynamic_cast<TupleInputNode *>(node)) {
            ret =  visit((TupleInputNode *) node);
        }
        else if (dynamic_cast<FunctionNode *>(node)) {
            ret =  visit((FunctionNode *) node);
        }
        else if (dynamic_cast<IndexNode *>(node)) {
            ret = visit((IndexNode *) node);
        }
        else if (dynamic_cast<IntervalNode *>(node)) {
            ret = visit((IntervalNode *) node);
        }
        else if (dynamic_cast<VectorNode *>(node)) {
            ret = visit((VectorNode *) node);
        }
        else if (dynamic_cast<MatrixNode *>(node)) {
            ret = visit((MatrixNode *) node);
        }
        else if (dynamic_cast<StreamStateNode *>(node)) {
            ret = visit((StreamStateNode *) node);
        }
        else if (dynamic_cast<ByNode *>(node)) {
            ret = visit((ByNode *) node);
        }
        else if (dynamic_cast<IntervalDeclNode *>(node)) {
            ret = visit((IntervalDeclNode *) node);
        }
        if (ret){
            node->setLlvmType(ret->getType());
        }

        return ret;
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
    llvm::Value* visit(NullNode *node) override { return nullptr;};
    llvm::Value* visit(CondNode *node) override { return nullptr;};
    llvm::Value* visit(LoopNode *node) override { return nullptr;};
    llvm::Value* visit(InLoopNode *node) override { return nullptr;};
    llvm::Value* visit(DoLoopNode *node) override { return nullptr;};
    llvm::Value* visit(DeclNode *node) override { return nullptr;};
    llvm::Value* visit(AssignNode *node) override { return nullptr;};
    llvm::Value* visit(CastExprNode *node) override { return nullptr;};
    llvm::Value* visit(IDNode *node) override { return nullptr;};
    llvm::Value* visit(IdnNode *node) override { return nullptr;};
    llvm::Value* visit(InputNode *node) override { return nullptr;};
    llvm::Value* visit(OutputNode *node) override { return nullptr;};
    llvm::Value* visit(StreamDeclNode *node) override { return nullptr;};
    llvm::Value* visit(TypeDefNode *node) override {return nullptr;};
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
    llvm::Value* visit(ProtoProcedureNode *node) override { return nullptr;};
    llvm::Value* visit(TupleDeclNode *node) override {return nullptr;}
    llvm::Value* visit(TupleType *node) override { return nullptr; }
    llvm::Value* visit(PythonTupleAssNode *node) override { return nullptr; }
    llvm::Value* visit(GlobalDeclNode *node) override { return nullptr; }
    llvm::Value* visit(VectorDeclNode *node) override { return nullptr; }
    llvm::Value* visit(MatrixDeclNode *node) override { return nullptr; }
    llvm::Value* visit(GlobalRefNode *node) override { return nullptr; }
    llvm::Value* visit(TupleNode *node) override { return nullptr; }
    llvm::Value* visit(IndexTupleNode *node) override { return nullptr; }
    llvm::Value* visit(TupleNode *node, llvm::StructType * tuple) override {return nullptr; }
    llvm::Value* visit(VectorType *node) override { return nullptr; }
    llvm::Value* visit(MatrixType *node) override { return nullptr; }
    llvm::Value* visit(TupleMemberAssNode *node) override { return nullptr; }
    llvm::Value* visit(TupleInputNode *node) override { return nullptr; }
    llvm::Value* visit(FunctionNode *node) override { return nullptr; }
    llvm::Value* visit(IndexNode *node) override { return nullptr; }
    llvm::Value* visit(IntervalNode *node) override { return nullptr; }
    llvm::Value* visit(VectorNode *node) override { return nullptr; }
    llvm::Value* visit(MatrixNode *node) override { return nullptr; }
    llvm::Value* visit(StreamStateNode *node) override { return nullptr; }
    llvm::Value* visit(ByNode *node) override { return nullptr; }
    llvm::Value* visit(IntervalDeclNode *node) override { return nullptr; }

};

#endif //GAZPREABASE_ASTBASEVISITOR_H
