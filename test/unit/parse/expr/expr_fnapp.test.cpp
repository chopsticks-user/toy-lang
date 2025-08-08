#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse::FnApp: Empty",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "foo()",

        .expectedAstPrint =
        "expr::FnApp [@0:0]\n"
        "├─ expr::Identifier [@0:0] with path = 'foo'\n"
        "├─ expr::Tuple [@0:3] with size = 0",

        .expectedPrettyPrint =
        "foo()",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::FnApp: 3 parameters",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "foo(x,y,z)",

        .expectedAstPrint =
        "expr::FnApp [@0:0]\n"
        "├─ expr::Identifier [@0:0] with path = 'foo'\n"
        "├─ expr::Tuple [@0:3] with size = 3\n"
        "   ├─ expr::Identifier [@0:4] with path = 'x'\n"
        "   ├─ expr::Identifier [@0:6] with path = 'y'\n"
        "   ├─ expr::Identifier [@0:8] with path = 'z'",

        .expectedPrettyPrint =
        "foo(x, y, z)",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse::FnApp: Currying",
    "[Unit][Parse][Expr]"
) {
    assertExpr({
        .source =
        "foo(x)(y)(z)(t)",

        .expectedAstPrint =
        "expr::FnApp [@0:0]\n" // foo(x)(y)(z)(t)
        "├─ expr::FnApp [@0:0]\n" // foo(x)(y)(z)
        "   ├─ expr::FnApp [@0:0]\n" // foo(x)(y)
        "      ├─ expr::FnApp [@0:0]\n" // foo(x)
        "         ├─ expr::Identifier [@0:0] with path = 'foo'\n"
        "         ├─ expr::Tuple [@0:3] with size = 1\n"
        "            ├─ expr::Identifier [@0:4] with path = 'x'\n"
        "      ├─ expr::Tuple [@0:6] with size = 1\n"
        "         ├─ expr::Identifier [@0:7] with path = 'y'\n"
        "   ├─ expr::Tuple [@0:9] with size = 1\n"
        "      ├─ expr::Identifier [@0:10] with path = 'z'\n"
        "├─ expr::Tuple [@0:12] with size = 1\n"
        "   ├─ expr::Identifier [@0:13] with path = 't'",

        .expectedPrettyPrint =
        "foo(x)(y)(z)(t)",
    });
}

// todo: error cases
