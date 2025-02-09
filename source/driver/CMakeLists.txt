llvm_map_components_to_libnames(llvm_libs support core irreader)

add_executable(tlc)
target_sources(
    tlc PRIVATE
    driver.cpp args.hpp args.cpp
)
target_include_directories(
    tlc PRIVATE
    ${LLVM_INCLUDE_DIRS}
)
target_link_libraries(
    tlc PRIVATE
    ${llvm_libs} tlc::core
)
set_target_properties(
    tlc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_NAME tlc
)