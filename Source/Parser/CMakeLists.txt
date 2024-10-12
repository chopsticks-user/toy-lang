add_executable(ToyLang_Parser)
add_executable(
    ${ToyLang_PROJECT_NAME}::Parser
    ALIAS ToyLang_Parser
)

target_sources(
    ToyLang_Parser
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/AST.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AST.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp
)
target_link_libraries(
    ToyLang_Parser
    PRIVATE
    ${ToyLang_PROJECT_NAME}::Core
)