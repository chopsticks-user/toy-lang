add_executable(ToyLang_UnitTestFrontEnd)
add_executable(
    ${ToyLang_PROJECT_NAME}::UnitTestFrontEnd
    ALIAS ToyLang_UnitTestFrontEnd
)
target_sources(
    ToyLang_UnitTestFrontEnd
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/TestLexer.cpp
)
#target_include_directories(ToyLang_UnitTestFrontEnd ${PROJECT_SOURCE_DIR}/Source)
target_link_libraries(
    ToyLang_UnitTestFrontEnd
    PRIVATE
    Catch2::Catch2WithMain
    ${ToyLang_PROJECT_NAME}::Core
    ${ToyLang_PROJECT_NAME}::FrontEnd
)

add_test(ToyLang_UnitTestFrontEnd ToyLang_UnitTestFrontEnd)