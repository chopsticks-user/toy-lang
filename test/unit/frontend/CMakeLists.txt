add_executable(tlc_test_unit_frontend)
add_executable(tlc::test::unit::frontend ALIAS tlc_test_unit_frontend)
target_sources(
    tlc_test_unit_frontend PRIVATE
    lex.test.cpp
    lex-stream.test.cpp
)
target_link_libraries(
    tlc_test_unit_frontend PRIVATE
    Catch2::Catch2WithMain tlc::lex
    Catch2::Catch2WithMain tlc::token
)
add_compile_definitions(
    TLC_TEST_SAMPLE_DIR="${PROJECT_SOURCE_DIR}/test/samples"
)
add_test(NAME tlc_test_unit_frontend COMMAND tlc_test_unit_frontend)