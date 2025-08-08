#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Defer: Single statement",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "defer io.println(x);",

        .expectedAstPrint =
        "stmt::Defer [@0:0]\n"
        "├─ stmt::Expression [@0:6]\n"
        "   ├─ expr::FnApp [@0:6]\n"
        "      ├─ expr::Identifier [@0:6] with path = 'io.println'\n"
        "      ├─ expr::Tuple [@0:16] with size = 1\n"
        "         ├─ expr::Identifier [@0:17] with path = 'x'",

        .expectedPrettyPrint =
        "defer io.println(x);",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Defer: Block statement",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "defer {};",

        .expectedAstPrint =
        "stmt::Defer [@0:0]\n"
        "├─ stmt::Block [@0:6] with size = 0",

        .expectedPrettyPrint =
        "defer {\n"
        "}",
    });
}
