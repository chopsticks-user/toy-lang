add_executable(ToyLang_Syntax)
add_executable(
    ${ToyLang_PROJECT_NAME}::Syntax
    ALIAS ToyLang_Syntax
)

target_sources(
    ToyLang_Syntax
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/AST.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/AST.cpp
)
#target_include_directories(
#    ToyLang_Syntax
#)
target_link_libraries(
    ToyLang_Syntax
    PRIVATE
    ${ToyLang_PROJECT_NAME}::Core
)