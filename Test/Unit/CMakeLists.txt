add_executable(UnitTest)
add_executable(
    ${ToyLang_PROJECT_NAME}::UnitTest
    ALIAS UnitTest
)

target_sources(
    UnitTest
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Test.cpp
)
#target_include_directories()
target_link_libraries(
    UnitTest
    PRIVATE
    spdlog::spdlog_header_only
    Catch2::Catch2WithMain
    nlohmann_json
    ${ToyLang_PROJECT_NAME}::Core
    ${ToyLang_PROJECT_NAME}::Lexer
    ${ToyLang_PROJECT_NAME}::Syntax
    ${ToyLang_PROJECT_NAME}::Parser
)

add_test(UnitTest UnitTest)