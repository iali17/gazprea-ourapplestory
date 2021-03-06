# Gather our source files in this directory.
set(
  gazprea_src_files
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/ExternalTools/ExternalTools.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/InternalTools/InternalTools.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Scope/Scope.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Scope/Symbol.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Scope/SymbolTable.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Scope/GazpreaType.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNodes/TerminalNodes/RealNode.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNodes/TerminalNodes/CharNode.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNodes/TerminalNodes/BoolNode.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNodes/BaseNodes/ArithNode.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNodes/BaseNodes/CompNode.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNodes/CompNodes/EQNode.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNodes/CompNodes/NEQNode.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNodes/CompNodes/GTNode.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNodes/CompNodes/LTNode.cpp"
        ../include/globals.h
	../include/AST/ASTNodes/BaseNodes/ASTNode.h
	../include/AST/AST.h
	AST/ASTNodes/BaseNodes/ASTNode.cpp
	../include/AST/ASTGenerator.h
        AST/ASTGenerator/ASTGenerator.cpp
	../include/AST/ASTNodes/BaseNodes/InfixNode.h
	../include/AST/ASTNodes/BaseNodes/UnaryNode.h
	AST/ASTNodes/BaseNodes/InfixNode.cpp
	AST/ASTNodes/BaseNodes/UnaryNode.cpp
	../include/AST/ASTNodes/BaseNodes/BlockNode.h
	AST/ASTNodes/BaseNodes/BlockNode.cpp
	../include/AST/ASTNodes/FileNode.h
	AST/ASTNodes/FileNode.cpp
	../include/CodeGenerator/CodeGenerator.h
	CodeGenerator/CodeGenerator.cpp
	../include/AST/ASTVisitor/ASTBaseVisitor.h
	../include/AST/ASTVisitor/ASTVisitor.h
	../include/InternalTools/CondBuilder.h
	InternalTools/CondBuilder.cpp
	globals.cpp
	../include/InternalTools/WhileBuilder.h
	InternalTools/WhileBuilder.cpp
	../include/AST/ASTNodes/StatementNodes/AssignNode.h
	../include/AST/ASTNodes/StatementNodes/DeclNode.h
	../include/AST/ASTNodes/TerminalNodes/IDNode.h
	../include/AST/ASTNodes/TerminalNodes/INTNode.h
	AST/ASTNodes/TerminalNodes/IDNode.cpp
	AST/ASTNodes/TerminalNodes/INTNode.cpp
	AST/ASTNodes/StatementNodes/AssignNode.cpp
	AST/ASTNodes/StatementNodes/DeclNode.cpp
	../include/AST/ASTNodes/FuncProcNodes/ProcedureNode.h
	AST/ASTNodes/FuncProcNodes/ProcedureNode.cpp
	../include/AST/ASTNodes/FuncProcNodes/ParamNode.h
	AST/ASTNodes/FuncProcNodes/ParamNode.cpp
	../include/AST/ASTNodes/StatementNodes/ReturnNode.h
	AST/ASTNodes/StatementNodes/ReturnNode.cpp
	../include/AST/ASTNodes/BaseNodes/BasicBlockNode.h
	AST/ASTNodes/BaseNodes/BasicBlockNode.cpp
	../include/InternalTools/CastTable.h
	InternalTools/CastTable.cpp
	../include/Scope/UserType.h
	Scope/UserType.cpp
	../include/AST/ASTNodes/StatementNodes/CondNode.h
	AST/ASTNodes/StatementNodes/CondNode.cpp
	../include/AST/ASTNodes/StatementNodes/LoopNodes/LoopNode.h
	../include/AST/ASTNodes/StatementNodes/LoopNodes/DoLoopNode.h
	../include/AST/ASTNodes/StatementNodes/LoopNodes/InLoopNode.h
	AST/ASTNodes/StatementNodes/LoopNodes/LoopNode.cpp
	AST/ASTNodes/StatementNodes/LoopNodes/InLoopNode.cpp
	AST/ASTNodes/StatementNodes/LoopNodes/DoLoopNode.cpp
	../include/AST/ASTNodes/BaseNodes/BaseLoopNode.h
	AST/ASTNodes/BaseNodes/BaseLoopNode.cpp
	../include/AST/ASTNodes/BaseNodes/BaseBlockParent.h
	AST/ASTNodes/BaseNodes/BaseBlockParent.cpp
	../include/AST/ASTNodes/TerminalNodes/InputNode.h
	../include/AST/ASTNodes/TerminalNodes/OutputNode.h
	../include/AST/ASTNodes/BaseNodes/IONode.h
	AST/ASTNodes/BaseNodes/IONode.cpp
	AST/ASTNodes/TerminalNodes/InputNode.cpp
	AST/ASTNodes/TerminalNodes/OutputNode.cpp
	../include/AST/ASTNodes/TerminalNodes/StreamDecl.h
	AST/ASTNodes/TerminalNodes/StreamDecl.cpp
	../include/AST/ASTNodes/ArithNodes/AddNode.h
	../include/AST/ASTNodes/ArithNodes/SubNode.h
	../include/AST/ASTNodes/ArithNodes/MulNode.h
	../include/AST/ASTNodes/ArithNodes/DivNode.h
	AST/ASTNodes/ArithNodes/AddNode.cpp
	AST/ASTNodes/ArithNodes/SubNode.cpp
	AST/ASTNodes/ArithNodes/MulNode.cpp
	AST/ASTNodes/ArithNodes/DivNode.cpp
	../include/AST/ASTNodes/FuncProcNodes/CallNode.h
	AST/ASTNodes/FuncProcNodes/CallNode.cpp
	../include/AST/ASTNodes/ArithNodes/RemNode.h
	AST/ASTNodes/ArithNodes/RemNode.cpp
	../include/AST/ASTNodes/CompNodes/NegateNode.h
	AST/ASTNodes/CompNodes/NegateNode.cpp
	../include/AST/ASTNodes/ArithNodes/AndNode.h
	../include/AST/ASTNodes/ArithNodes/OrNode.h
	../include/AST/ASTNodes/ArithNodes/XOrNode.h
	AST/ASTNodes/ArithNodes/AndNode.cpp
	AST/ASTNodes/ArithNodes/OrNode.cpp
	AST/ASTNodes/ArithNodes/XOrNode.cpp
	../include/AST/ASTNodes/BaseNodes/BitArithNode.h
	AST/ASTNodes/BaseNodes/BitArithNode.cpp
	../include/AST/ASTNodes/CompNodes/GTENode.h
	../include/AST/ASTNodes/CompNodes/LTENode.h
	AST/ASTNodes/CompNodes/GTENode.cpp
	AST/ASTNodes/CompNodes/LTENode.cpp
	CodeGenerator/Operations.cpp
		../include/AST/ASTNodes/StatementNodes/CastNodes/CastExprNode.h
        AST/ASTNodes/StatementNodes/CastNodes/CastExprNode.cpp
	AST/ASTNodes/TerminalNodes/TupleNode.cpp
	../include/AST/ASTNodes/TerminalNodes/TupleNode.h
	AST/ASTNodes/StatementNodes/TupleDeclNode.cpp
	../include/AST/ASTNodes/StatementNodes/TupleDeclNode.h
	../include/AST/ASTNodes/StatementNodes/BreakNode.h
	../include/AST/ASTNodes/StatementNodes/ContinueNode.h
	AST/ASTNodes/StatementNodes/ContinueNode.cpp
	AST/ASTNodes/StatementNodes/BreakNode.cpp
	../include/AST/ASTNodes/ArithNodes/ExpNode.h
	AST/ASTNodes/ArithNodes/ExpNode.cpp
		AST/ASTNodes/TypeNodes/TupleTypeNode.cpp
		../include/AST/ASTNodes/TypeNodes/TupleTypeNode.h
	CodeGenerator/HelperFunctions.cpp
	../include/Scope/FunctionSymbol.h
	Scope/FunctionSymbol.cpp
	CodeGenerator/Procedures.cpp
	Scope/GazpreaTupleType.cpp
	../include/Scope/GazpreaTupleType.h
	../include/AST/ASTNodes/FuncProcNodes/ProtoProcedureNode.h
	AST/ASTNodes/FuncProcNodes/ProtoProcedureNode.cpp
	../include/AST/ASTNodes/StatementNodes/GlobalDeclNode.h
	AST/ASTNodes/StatementNodes/GlobalDeclNode.cpp
	../include/AST/ASTNodes/TerminalNodes/GlobalRefNode.h
	AST/ASTNodes/TerminalNodes/GlobalRefNode.cpp
	../include/InternalTools/ErrorBuilder.h
	../include/AST/ASTNodes/ErrorNodes/ErrorNode.h
        ../include/AST/ASTNodes/ErrorNodes/ScalarErrorNode.h
        AST/ASTNodes/ErrorNodes/ScalarErrorNode.cpp
	InternalTools/ErrorBuilder.cpp
	AST/ASTNodes/StatementNodes/PythonTupleAssNode.cpp
	../include/AST/ASTNodes/StatementNodes/PythonTupleAssNode.h
	../include/AST/ASTNodes/TerminalNodes/IndexTupleNode.h
	AST/ASTNodes/TerminalNodes/IndexTupleNode.cpp
	../include/AST/ASTNodes/TerminalNodes/NullNode.h
	AST/ASTNodes/TerminalNodes/NullNode.cpp
	AST/ASTNodes/TerminalNodes/IdnNode.cpp
	../include/AST/ASTNodes/TerminalNodes/IdnNode.h
	AST/ASTNodes/TerminalNodes/TypeDefNode.cpp
	../include/AST/ASTNodes/TerminalNodes/TypeDefNode.h
	../include/AST/ASTNodes/StatementNodes/TupleMemberAssNode.h
	AST/ASTNodes/StatementNodes/TupleMemberAssNode.cpp
	../include/AST/ASTNodes/TerminalNodes/TupleInputNode.h
	AST/ASTNodes/TerminalNodes/TupleInputNode.cpp
	../include/AST/ASTNodes/FuncProcNodes/FunctionNode.h
	AST/ASTNodes/FuncProcNodes/FunctionNode.cpp
	CodeGenerator/Tuples.cpp
	../include/AST/ASTNodes/TerminalNodes/IndexNode.h
	AST/ASTNodes/TerminalNodes/IndexNode.cpp
	../include/AST/ASTNodes/TerminalNodes/IntervalNode.h
	AST/ASTNodes/TerminalNodes/IntervalNode.cpp
	ExternalTools/RTVectorTools.cpp
	ExternalTools/RTIntervalTools.cpp
		AST/ASTGenerator/HelperFunctions.cpp
	AST/ASTNodes/TerminalNodes/VectorNode.cpp
	../include/AST/ASTNodes/TerminalNodes/VectorNode.h
	CodeGenerator/DeclAss.cpp
	../include/AST/ASTNodes/StatementNodes/VectorDeclNode.h
	AST/ASTNodes/StatementNodes/VectorDeclNode.cpp
	AST/ASTNodes/TerminalNodes/MatrixNode.cpp
	../include/AST/ASTNodes/TerminalNodes/MatrixNode.h
	AST/ASTNodes/StatementNodes/MatrixDeclNode.cpp
	../include/AST/ASTNodes/StatementNodes/MatrixDeclNode.h
        AST/ASTNodes/TypeNodes/MatrixTypeNode.cpp
        ../include/AST/ASTNodes/TypeNodes/MatrixTypeNode.h
	../include/AST/ASTNodes/BaseNodes/BuiltIn.h
	AST/ASTNodes/BaseNodes/BuiltIn.cpp
	AST/ASTNodes/BuiltInFuncNode/StreamStateNode.cpp
	../include/AST/ASTNodes/BuiltInFuncNodes/StreamStateNode.h
	../include/AST/ASTNodes/BuiltInFuncNodes/StreamStateNode.h
	ExternalTools/RTMatrixTools.cpp
		../include/AST/ASTNodes/TypeNodes/VectorTypeNode.h
		AST/ASTNodes/TypeNodes/VectorTypeNode.cpp
	CodeGenerator/Vectors.cpp
	CodeGenerator/Matrices.cpp
	CodeGenerator/Casts.cpp
	../include/AST/ASTNodes/StatementNodes/CastNodes/ScalarCastNode.h
	../include/AST/ASTNodes/StatementNodes/CastNodes/TupleCastNode.h
	AST/ASTNodes/StatementNodes/CastNodes/ScalarCastNode.cpp
	AST/ASTNodes/StatementNodes/CastNodes/TupleCastNode.cpp
	../include/AST/ASTNodes/StatementNodes/CastNodes/VectorCastNode.h
	AST/ASTNodes/StatementNodes/CastNodes/VectorCastNode.cpp
	../include/AST/ASTNodes/StatementNodes/CastNodes/MatrixCastNode.h
	AST/ASTNodes/StatementNodes/CastNodes/MatrixCastNode.cpp
	AST/ASTNodes/StatementNodes/CastNodes/ByNode.cpp
	../include/AST/ASTNodes/StatementNodes/CastNodes/ByNode.h
	AST/ASTNodes/StatementNodes/IntervalDeclNode.cpp
	../include/AST/ASTNodes/StatementNodes/IntervalDeclNode.h

	CodeGenerator/Intervals.cpp ../include/AST/ASTNodes/BuiltInFuncNodes/ReverseVectorNode.h AST/ASTNodes/BuiltInFuncNode/ReverseVectorNode.cpp ../include/AST/ASTNodes/StatementNodes/GeneratorNode.h AST/ASTNodes/StatementNodes/GeneratorNode.cpp CodeGenerator/GeneratorsAndFilters.cpp AST/ASTGenerator/DeclAss.cpp AST/ASTGenerator/Tuples.cpp AST/ASTGenerator/ProceduresAndFunctions.cpp AST/ASTGenerator/Vectors.cpp AST/ASTGenerator/Matrices.cpp AST/ASTGenerator/Intervals.cpp CodeGenerator/VectorOperations.cpp ../include/AST/ASTNodes/ArithNodes/ConcatenationNode.h AST/ASTNodes/ArithNodes/ConcatenationNode.cpp ../include/AST/ASTNodes/ArithNodes/DotProductNode.h AST/ASTNodes/ArithNodes/DotProductNode.cpp ../include/AST/ASTNodes/ErrorNodes/VectorErrorNode.h AST/ASTNodes/ErrorNodes/VectorErrorNode.cpp AST/ASTNodes/ErrorNodes/ErrorNode.cpp  ../include/AST/ASTNodes/StatementNodes/FilterNode.h AST/ASTNodes/StatementNodes/FilterNode.cpp ../include/AST/ASTNodes/BuiltInFuncNodes/LengthNode.h AST/ASTNodes/BuiltInFuncNode/LengthNode.cpp ../include/AST/ASTNodes/StatementNodes/SliceAssignNode.h AST/ASTNodes/StatementNodes/SliceAssignNode.cpp ../include/AST/ASTNodes/TerminalNodes/StringNode.h AST/ASTNodes/TerminalNodes/StringNode.cpp ../include/AST/ASTNodes/TerminalNodes/IndexFilterNode.h AST/ASTNodes/TerminalNodes/IndexFilterNode.cpp ../include/AST/ASTNodes/TerminalNodes/IndexFilterNode.h AST/ASTNodes/TerminalNodes/IndexFilterNode.cpp CodeGenerator/MatrixOperations.cpp ../include/AST/ASTNodes/BuiltInFuncNodes/RowsNode.h ../include/AST/ASTNodes/BuiltInFuncNodes/ColsNode.h AST/ASTNodes/BuiltInFuncNode/RowsNode.cpp AST/ASTNodes/BuiltInFuncNode/ColsNode.cpp)


# Build our executable from the source files.
add_executable(gazc ${gazprea_src_files})
target_include_directories(gazc PUBLIC ${ANTLR_GEN_DIR})

# Ensure that the antlr4-runtime is available.
add_dependencies(gazc antlr)

# Get llvm components.
llvm_map_components_to_libnames(llvm_libs core)

# Add the antlr runtime and parser as libraries to link.
target_link_libraries(gazc parser antlr4-runtime ${llvm_libs})

# Symbolic link our executable to the base directory so we don't have to go searching for it.
symlink_to_bin("gazc")
