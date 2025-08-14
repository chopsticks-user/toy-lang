# LLVM
find_package(LLVM ${LLVM_VERSION} REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(llvm_libs support core irreader)
#    PRIVATE ${llvm_libs}
