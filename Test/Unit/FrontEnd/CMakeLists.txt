add_executable(ToyLang_UnitTestFrontEnd)

add_executable(
    ${ToyLang_PROJECT_NAME}::UnitTestFrontEnd
    ALIAS ToyLang_UnitTestFrontEnd
)

target_sources(
    ToyLang_UnitTestFrontEnd
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Lexer.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Parser.test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PrettyPrinter.test.cpp
)

#target_include_directories(ToyLang_UnitTestFrontEnd ${PROJECT_SOURCE_DIR}/Source)

target_link_libraries(
    ToyLang_UnitTestFrontEnd
    PRIVATE
    Catch2::Catch2WithMain
    ${ToyLang_PROJECT_NAME}::Core
    ${ToyLang_PROJECT_NAME}::FrontEnd
    ${ToyLang_PROJECT_NAME}::Syntax
)

add_compile_definitions(RESOURCE_DIR="${PROJECT_SOURCE_DIR}/Test/Samples")

add_test(
    NAME ToyLang_UnitTestFrontEnd
    COMMAND ToyLang_UnitTestFrontEnd
)