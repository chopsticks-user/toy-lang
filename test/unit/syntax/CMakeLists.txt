add_executable(tlc_test_unit_syntax)
add_executable(tlc::test::unit::syntax ALIAS tlc_test_unit_syntax)
target_sources(
    tlc_test_unit_syntax PRIVATE
    syntax.test.cpp
)
target_link_libraries(
    tlc_test_unit_syntax PRIVATE
    Catch2::Catch2WithMain tlc::syntax
    Catch2::Catch2WithMain tlc::token
)
add_test(NAME tlc_test_unit_syntax COMMAND tlc_test_unit_syntax)