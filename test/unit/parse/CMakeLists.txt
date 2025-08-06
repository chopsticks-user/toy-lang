add_executable(tlc_test_unit_parse)
add_executable(tlc::test::unit::parse ALIAS tlc_test_unit_parse)
target_sources(
    tlc_test_unit_parse PRIVATE
    parse.test.hpp
    parse.test.cpp

    token_stream.test.cpp
    combinator.test.cpp

    expr_literal.test.cpp
    expr_id.test.cpp
    expr_tuple.test.cpp
    expr_array.test.cpp
    expr_record.test.cpp
    expr_try.test.cpp
    expr_string.test.cpp
    expr_fnapp.test.cpp
    expr_subscript.test.cpp
    expr_prefix.test.cpp
    expr_binary.test.cpp

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
