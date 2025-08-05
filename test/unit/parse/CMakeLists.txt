add_executable(tlc_test_unit_parse)
add_executable(tlc::test::unit::parse ALIAS tlc_test_unit_parse)
target_sources(
    tlc_test_unit_parse PRIVATE
    token_stream.test.cpp
    combinator.test.cpp
    parse.test.hpp
    expr/tuple.test.cpp
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
target_include_directories(
    tlc_test_unit_parse PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_test(NAME tlc_test_unit_parse COMMAND tlc_test_unit_parse)

add_executable(tlc_test_experiment)
target_sources(tlc_test_experiment PRIVATE test.cpp)
target_link_libraries(
    tlc_test_experiment PRIVATE
    Catch2::Catch2WithMain tlc::core
)
add_test(NAME tlc_test_experiment COMMAND tlc_test_experiment)
