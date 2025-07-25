add_executable(tlc_test_unit_token)
add_executable(tlc::test::unit::token ALIAS tlc_test_unit_token)
target_sources(
    tlc_test_unit_token PRIVATE
    token.test.cpp
)
target_link_libraries(
    tlc_test_unit_token PRIVATE
    Catch2::Catch2WithMain tlc::token
)
add_test(NAME tlc_test_unit_token COMMAND tlc_test_unit_token)