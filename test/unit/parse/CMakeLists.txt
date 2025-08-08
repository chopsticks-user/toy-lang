add_executable(tlc_test_unit_parse)
add_executable(tlc::test::unit::parse ALIAS tlc_test_unit_parse)
target_sources(
    tlc_test_unit_parse PRIVATE
    token_stream.test.cpp
    combinator.test.cpp
    parse.test.hpp
    parse.test.cpp

    expr/expr_literal.test.cpp
    expr/expr_id.test.cpp
    expr/expr_tuple.test.cpp
    expr/expr_array.test.cpp
    expr/expr_record.test.cpp
    expr/expr_try.test.cpp
    expr/expr_string.test.cpp
    expr/expr_fnapp.test.cpp
    expr/expr_subscript.test.cpp
    expr/expr_prefix.test.cpp
    expr/expr_binary.test.cpp

    type/type_array.test.cpp
    type/type_function.test.cpp
    type/type_generic.test.cpp
    type/type_id.test.cpp
    type/type_tuple.test.cpp
    type/type_binary.test.cpp

    decl/decl_generic.test.cpp
    decl/decl_id.test.cpp
    decl/decl_tuple.test.cpp

    global/global_module.test.cpp
    global/global_import.test.cpp
)
target_link_libraries(
    tlc_test_unit_parse PRIVATE
    Catch2::Catch2WithMain tlc::lex tlc::parse
)
target_include_directories(
    tlc_test_unit_parse PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_test(NAME tlc_test_unit_parse COMMAND tlc_test_unit_parse)
