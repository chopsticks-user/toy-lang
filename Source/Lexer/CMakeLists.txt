add_library(ToyLang_Lexer)
add_library(
    ${ToyLang_PROJECT_NAME}::Lexer
    ALIAS ToyLang_Lexer
)

target_sources(
    ToyLang_Lexer
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Token.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Token.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Lexer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Lexer.cpp
)
#target_include_directories(
#    ToyLang_Lexer
#)
target_link_libraries(
    ToyLang_Lexer
    PRIVATE
    ${ToyLang_PROJECT_NAME}::Core
)