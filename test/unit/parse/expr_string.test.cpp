#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse::String: Empty",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "\"\"",

        .expectedAstPrint =
        "expr::String [@0:0] with nPlaceholders = 0",

        .expectedPrettyPrint =
        "\"\"",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::String: Non-interpolated string",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "\"just\\t a normal string\\n\"",

        .expectedAstPrint =
        "expr::String [@0:0] with nPlaceholders = 0",

        .expectedPrettyPrint =
        "\"just\t a normal string\n\"",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::String: 4 fragments, 3 placeholders",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "\"Array: [{0}, { x }, { false}]\"",

        .expectedAstPrint =
        "expr::String [@0:0] with nPlaceholders = 3\n"
        "├─ expr::Integer [@0:10] with value = 0\n"
        "├─ expr::Identifier [@0:16] with path = 'x'\n"
        "├─ expr::Boolean [@0:23] with value = false",

        .expectedPrettyPrint =
        "\"Array: [{0}, {x}, {false}]\"",
    });
}

// todo: errors cases
