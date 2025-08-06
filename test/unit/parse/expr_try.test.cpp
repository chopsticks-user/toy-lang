#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse::Try: Empty",
    "[Unit][Parse][Expr]"
) {
    assertExprWithParams({
        .source =
        "try",

        .expectedAstPrint =
        "expr::Try [@0:0]\n"
        "├─ (required)",

        .expectedPrettyPrint =
        "try {!}",

        .expectedErrors = {
            {.context = Context::TryExpr, .reason = Reason::MissingExpr}
        },
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::Try: Identifier as operand",
    "[Unit][Parse][Expr]"
) {
    assertExprWithParams({
        .source =
        "try foo.bar.baz",

        .expectedAstPrint =
        "expr::Try [@0:0]\n"
        "├─ expr::Identifier [@0:4] with path = 'foo.bar.baz'",

        .expectedPrettyPrint =
        "try foo.bar.baz",
    });
}
