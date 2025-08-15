add_executable(tlc_test_unit_static)
add_executable(tlc::test::unit::static ALIAS tlc_test_unit_static)
target_sources(
    tlc_test_unit_static PRIVATE
    static.test.hpp static.test.cpp

    post_parsing/tuple_reducer.test.cpp
)
target_link_libraries(
    tlc_test_unit_static PRIVATE
    Catch2::Catch2WithMain tlc::lex tlc::parse tlc::static
)
target_include_directories(
    tlc_test_unit_static PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
add_test(NAME tlc_test_unit_static COMMAND tlc_test_unit_static)