# Gather our source files in this directory.
set(
  gazprea_rt_files
  "rt_source/vector_tools.c"
  "rt_source/interval_tools.c"
  "rt_source/matrix_tools.c")

# Build our executable from the source files.
add_library(gazrt SHARED ${gazprea_rt_files})
target_include_directories(gazc PUBLIC ${RUNTIME_INCLUDE})

# Symbolic link our library to the base directory so we don't have to go searching for it.
symlink_to_bin("gazrt")
