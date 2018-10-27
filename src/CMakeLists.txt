# Gather our source files in this directory.
set(
  gazprea_src_files
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/ExternalTools/ExternalTools.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/InternalTools/InternalTools.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Scope/Scope.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Scope/Symbol.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Scope/SymbolTable.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNodes/TerminalNodes/RealNode.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNodes/TerminalNodes/CharNode.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/AST/ASTNodes/TerminalNodes/BoolNode.cpp"
        ../include/globals.h ../include/AST/ASTNodes/BaseNodes/ASTNode.h ../include/AST/AST.h AST/ASTNodes/BaseNodes/ASTNode.cpp ../include/AST/ASTGenerator.h AST/ASTGenerator.cpp ../include/AST/ASTNodes/BaseNodes/InfixNode.h ../include/AST/ASTNodes/BaseNodes/UnaryNode.h AST/ASTNodes/BaseNodes/InfixNode.cpp AST/ASTNodes/BaseNodes/UnaryNode.cpp ../include/AST/ASTNodes/BaseNodes/BlockNode.h AST/ASTNodes/BaseNodes/BlockNode.cpp ../include/AST/ASTNodes/FileNode.h AST/ASTNodes/FileNode.cpp ../include/CodeGenerator/CodeGenerator.h CodeGenerator/CodeGenerator.cpp ../include/AST/ASTVisitor/ASTBaseVisitor.h ../include/AST/ASTVisitor/ASTVisitor.h ../include/InternalTools/CondBuilder.h InternalTools/CondBuilder.cpp globals.cpp ../include/InternalTools/WhileBuilder.h InternalTools/WhileBuilder.cpp ../include/AST/ASTNodes/StatementNodes/AssignNode.h ../include/AST/ASTNodes/StatementNodes/DeclNode.h ../include/AST/ASTNodes/TerminalNodes/IDNode.h ../include/AST/ASTNodes/TerminalNodes/INTNode.h AST/ASTNodes/TerminalNodes/IDNode.cpp AST/ASTNodes/TerminalNodes/INTNode.cpp AST/ASTNodes/StatementNodes/AssignNode.cpp AST/ASTNodes/StatementNodes/DeclNode.cpp ../include/AST/ASTNodes/FuncProcNodes/ProcedureNode.h AST/ASTNodes/FuncProcNodes/ProcedureNode.cpp ../include/AST/ASTNodes/FuncProcNodes/ParamNode.h AST/ASTNodes/FuncProcNodes/ParamNode.cpp ../include/AST/ASTNodes/StatementNodes/ReturnNode.h AST/ASTNodes/StatementNodes/ReturnNode.cpp ../include/AST/ASTNodes/BaseNodes/BasicBlockNode.h AST/ASTNodes/BaseNodes/BasicBlockNode.cpp ../include/InternalTools/CastTable.h InternalTools/CastTable.cpp ../include/Scope/UserType.h Scope/UserType.cpp ../include/AST/ASTNodes/StatementNodes/CondNode.h AST/ASTNodes/StatementNodes/CondNode.cpp)

# Build our executable from the source files.
add_executable(gazc ${gazprea_src_files})
target_include_directories(gazc PUBLIC ${ANTLR_GEN_DIR})

# Ensure that the antlr4-runtime is available.
add_dependencies(gazc antlr)

# Add the antlr runtime and parser as libraries to link.
target_link_libraries(gazc parser antlr4-runtime)

# Get llvm components.
llvm_map_components_to_libnames(llvm_libs core)

# Add the antlr runtime and parser as libraries to link.
target_link_libraries(gazc parser antlr4-runtime ${llvm_libs})

# Symbolic link our executable to the base directory so we don't have to go searching for it.
symlink_to_bin("gazc")
