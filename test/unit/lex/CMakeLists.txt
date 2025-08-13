add_executable(tlc_test_unit_lex)
add_executable(tlc::test::unit::lex ALIAS tlc_test_unit_lex)
target_sources(
    tlc_test_unit_lex PRIVATE
    lex.test.cpp
    text_stream.test.cpp
)
target_link_libraries(
    tlc_test_unit_lex
    PRIVATE Catch2::Catch2WithMain tlc::lex tlc::token
)
add_test(NAME tlc_test_unit_lex COMMAND tlc_test_unit_lex)