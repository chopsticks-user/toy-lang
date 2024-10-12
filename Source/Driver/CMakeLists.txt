add_executable(ToyLang_Driver)
add_executable(
    ${ToyLang_PROJECT_NAME}::Driver
    ALIAS ToyLang_Driver
)

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_sources(
    ToyLang_Driver
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Driver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ConfigParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ConfigParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandParser.cpp
)
target_include_directories(
    ToyLang_Driver
    PRIVATE ${LLVM_INCLUDE_DIRS}
)
target_link_libraries(
    ToyLang_Driver
    PRIVATE
    ${llvm_libs}
    ${ToyLang_PROJECT_NAME}::Core
    ${ToyLang_PROJECT_NAME}::Lexer
    ${ToyLang_PROJECT_NAME}::Parser
    nlohmann_json
)
set_target_properties(
    ToyLang_Driver PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_NAME tlc
)