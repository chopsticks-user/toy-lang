add_library(ToyLang_Syntax SHARED)
add_library(
    ${ToyLang_PROJECT_NAME}::Syntax
    ALIAS ToyLang_Syntax
)

target_sources(
    ToyLang_Syntax
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Forward.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Base.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Base.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Expressions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Expressions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Statements.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Statements.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Definitions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Definitions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SyntaxTreeVisitor.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Syntax.hpp
)
target_link_libraries(
    ToyLang_Syntax
    PRIVATE
    ${ToyLang_PROJECT_NAME}::Core
)