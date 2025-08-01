llvm_map_components_to_libnames(llvm_libs support core irreader)

add_library(tlc_driver SHARED)
add_library(tlc::driver ALIAS tlc_driver)
target_sources(
    tlc_driver PRIVATE
    command.hpp command.cpp project.hpp project.cpp driver.hpp driver.cpp
)
target_include_directories(
    tlc_driver PRIVATE
    ${LLVM_INCLUDE_DIRS}
)
target_link_libraries(
    tlc_driver
    PUBLIC tlc::core
    #    PRIVATE ${llvm_libs}
)

add_executable(tlc)
target_sources(
    tlc PRIVATE
    tlc.cpp
)
target_link_libraries(
    tlc PRIVATE
    tlc::core tlc::token tlc::lex tlc::syntax tlc::driver
)
set_target_properties(
    tlc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_NAME tlc
)