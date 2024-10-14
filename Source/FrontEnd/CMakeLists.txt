add_library(ToyLang_FrontEnd SHARED)
add_library(
    ${ToyLang_PROJECT_NAME}::FrontEnd
    ALIAS ToyLang_FrontEnd
)
target_sources(
    ToyLang_FrontEnd
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/FrontEnd.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Token.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Token.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Lexer.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Lexer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PrettyPrinter.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PrettyPrinter.cpp
)
target_link_libraries(
    ToyLang_FrontEnd
    PRIVATE
    ${ToyLang_PROJECT_NAME}::Core
)