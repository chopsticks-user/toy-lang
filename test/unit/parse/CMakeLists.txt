add_executable(tlc_test_unit_parse)
add_executable(tlc::test::unit::parse ALIAS tlc_test_unit_parse)
target_sources(
    tlc_test_unit_parse PRIVATE
    token_stream.test.cpp
    combinator.test.cpp
    parse.test.hpp
    handle_expr.test.cpp
    handle_type.test.cpp
    handle_decl.test.cpp
    handle_stmt.test.cpp
    handle_global_decl.test.cpp
    handle_function_def.test.cpp
)
target_link_libraries(
    tlc_test_unit_parse PRIVATE
    Catch2::Catch2WithMain tlc::lex tlc::parse
)
add_test(NAME tlc_test_unit_parse COMMAND tlc_test_unit_parse)