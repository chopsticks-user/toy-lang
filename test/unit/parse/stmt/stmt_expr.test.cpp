#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Expr: Empty",
    "[Unit][Parse][Stmt]"
) {
    // assertStmt({
    //     .source =
    //     ";",
    //
    //     .expectedAstPrint =
    //     "stmt::Expression [@0:0]\n"
    //     "├─ expr::FnApp [@0:0]\n"
    //     "   ├─ expr::Identifier [@0:0] with path = 'foo.bar'\n"
    //     "   ├─ expr::Tuple [@0:7] with size = 0",
    //
    //     .expectedPrettyPrint =
    //     ";",
    // });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Expr: Function call",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "foo.bar();",

        .expectedAstPrint =
        "stmt::Expression [@0:0]\n"
        "├─ expr::FnApp [@0:0]\n"
        "   ├─ expr::Identifier [@0:0] with path = 'foo.bar'\n"
        "   ├─ expr::Tuple [@0:7] with size = 0",

        .expectedPrettyPrint =
        "foo.bar();",
    });
}
