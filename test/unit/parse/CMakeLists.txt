add_executable(tlc_test_unit_parse)
add_executable(tlc::test::unit::parse ALIAS tlc_test_unit_parse)
target_sources(
    tlc_test_unit_parse PRIVATE
    parse.test.cpp
    stream.test.cpp
    combinators.test.cpp
)
target_link_libraries(
    tlc_test_unit_parse PRIVATE
    Catch2::Catch2WithMain tlc::lex tlc::parse
)
add_test(NAME tlc_test_unit_parse COMMAND tlc_test_unit_parse)