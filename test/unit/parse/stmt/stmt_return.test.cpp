#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Return: Empty",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "return;",

        .expectedAstPrint =
        "stmt::Return [@0:0]\n"
        "├─ (empty)",

        .expectedPrettyPrint =
        "return {?};",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Return: Single-value return",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "return x;",

        .expectedAstPrint =
        "stmt::Return [@0:0]\n"
        "├─ expr::Identifier [@0:7] with path = 'x'",

        .expectedPrettyPrint =
        "return x;",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Return: Tuple return",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "return (foo.bar, 0);",

        .expectedAstPrint =
        "stmt::Return [@0:0]\n"
        "├─ expr::Tuple [@0:7] with size = 2\n"
        "   ├─ expr::Identifier [@0:8] with path = 'foo.bar'\n"
        "   ├─ expr::Integer [@0:17] with value = 0",

        .expectedPrettyPrint =
        "return (foo.bar, 0);",
    });
}
