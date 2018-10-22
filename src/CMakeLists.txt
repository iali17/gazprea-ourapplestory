# Gather our source files in this directory.
set(
  gazprea_src_files
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/ExternalTools/ExternalTools.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/InternalTools/InternalTools.cpp"
        ../include/globals.h ../include/AST/ASTNodes/BaseNodes/ASTNode.h ../include/AST/AST.h AST/ASTNodes/BaseNodes/ASTNode.cpp ../include/AST/ASTGenerator.h AST/ASTGenerator.cpp ../include/AST/ASTNodes/BaseNodes/InfixNode.h ../include/AST/ASTNodes/BaseNodes/UnaryNode.h AST/ASTNodes/BaseNodes/InfixNode.cpp AST/ASTNodes/BaseNodes/UnaryNode.cpp ../include/AST/ASTNodes/BaseNodes/BlockNode.h AST/ASTNodes/BaseNodes/BlockNode.cpp ../include/AST/ASTNodes/FileNode.h AST/ASTNodes/FileNode.cpp ../include/CodeGenerator/CodeGenerator.h CodeGenerator/CodeGenerator.cpp ../include/AST/ASTVisitor/ASTBaseVisitor.h ../include/AST/ASTVisitor/ASTVisitor.h ../include/InternalTools/CondBuilder.h InternalTools/CondBuilder.cpp globals.cpp ../include/InternalTools/WhileBuilder.h InternalTools/WhileBuilder.cpp ../include/AST/ASTNodes/AssignNode.h ../include/AST/ASTNodes/DeclNode.h ../include/AST/ASTNodes/BaseNodes/IDNode.h ../include/AST/ASTNodes/BaseNodes/INTNode.h AST/ASTNodes/BaseNodes/IDNode.cpp AST/ASTNodes/BaseNodes/INTNode.cpp AST/ASTNodes/AssignNode.cpp AST/ASTNodes/DeclNode.cpp ../include/AST/ASTNodes/ProcedureNode.h AST/ASTNodes/ProcedureNode.cpp ../include/AST/ASTNodes/ParamNode.h AST/ASTNodes/ParamNode.cpp ../include/AST/ASTNodes/ReturnNode.h AST/ASTNodes/ReturnNode.cpp ../include/AST/ASTNodes/BaseNodes/BasicBlockNode.h AST/ASTNodes/BaseNodes/BasicBlockNode.cpp)

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
