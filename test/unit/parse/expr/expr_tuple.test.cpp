#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse::Tuple: Empty",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "()",

        .expectedAstPrint =
        "expr::Tuple [@0:0] with size = 0",

        .expectedPrettyPrint =
        "()",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Tuple: 1 argument",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "(x)",

        .expectedAstPrint =
        "expr::Tuple [@0:0] with size = 1\n"
        "├─ expr::Identifier [@0:1] with path = 'x'",

        .expectedPrettyPrint =
        "(x)",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Tuple: 3 arguments",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "(1,x,2.1)",

        .expectedAstPrint =
        "expr::Tuple [@0:0] with size = 3\n"
        "├─ expr::Integer [@0:1] with value = 1\n"
        "├─ expr::Identifier [@0:3] with path = 'x'\n"
        "├─ expr::Float [@0:5] with value = 2.1",

        .expectedPrettyPrint =
        "(1, x, 2.1)",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Tuple: Nested",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "(((())), 0,x,(1, (2)))",

        .expectedAstPrint =
        "expr::Tuple [@0:0] with size = 4\n"
        "├─ expr::Tuple [@0:1] with size = 1\n"
        "   ├─ expr::Tuple [@0:2] with size = 1\n"
        "      ├─ expr::Tuple [@0:3] with size = 0\n"
        "├─ expr::Integer [@0:9] with value = 0\n"
        "├─ expr::Identifier [@0:11] with path = 'x'\n"
        "├─ expr::Tuple [@0:13] with size = 2\n"
        "   ├─ expr::Integer [@0:14] with value = 1\n"
        "   ├─ expr::Tuple [@0:17] with size = 1\n"
        "      ├─ expr::Integer [@0:18] with value = 2",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Tuple: Missing an argument",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "(1,,x)",

        .expectedAstPrint =
        "expr::Tuple [@0:0] with size = 3\n"
        "├─ expr::Integer [@0:1] with value = 1\n"
        "├─ (required)\n"
        "├─ expr::Identifier [@0:4] with path = 'x'",

        .expectedErrors = {
            {.context = Context::Tuple, .reason = Reason::MissingExpr},
        },
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Tuple: Missing all arguments",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "(,,,,)",

        .expectedAstPrint =
        "expr::Tuple [@0:0] with size = 5\n"
        "├─ (required)\n"
        "├─ (required)\n"
        "├─ (required)\n"
        "├─ (required)\n"
        "├─ (required)",

        .expectedErrors = {
            {.context = Context::Tuple, .reason = Reason::MissingExpr},
            {.context = Context::Tuple, .reason = Reason::MissingExpr},
            {.context = Context::Tuple, .reason = Reason::MissingExpr},
            {.context = Context::Tuple, .reason = Reason::MissingExpr},
            {.context = Context::Tuple, .reason = Reason::MissingExpr},
        },
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Tuple: Missing the enclosing parenthesis",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "(1,0.1,x",

        .expectedAstPrint =
        "expr::Tuple [@0:0] with size = 3\n"
        "├─ expr::Integer [@0:1] with value = 1\n"
        "├─ expr::Float [@0:3] with value = 0.1\n"
        "├─ expr::Identifier [@0:7] with path = 'x'",

        .expectedErrors = {
            {.context = Context::Tuple, .reason = Reason::MissingEnclosingSymbol},
        },
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Tuple: Missing an argument and the enclosing parenthesis",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "(1,,x",

        .expectedAstPrint =
        "expr::Tuple [@0:0] with size = 3\n"
        "├─ expr::Integer [@0:1] with value = 1\n"
        "├─ (required)\n"
        "├─ expr::Identifier [@0:4] with path = 'x'",

        .expectedErrors = {
            {.context = Context::Tuple, .reason = Reason::MissingExpr},
            {.context = Context::Tuple, .reason = Reason::MissingEnclosingSymbol},
        },
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Tuple: Not a tuple if missing the opening parenthesis",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "1,0.1,x)",

        .expectedAstPrint =
        "expr::Integer [@0:0] with value = 1",
    });
}
