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
    INTERFACE ${ToyLang_PROJECT_NAME}::CFlags
)

add_test(UnitTest UnitTest)