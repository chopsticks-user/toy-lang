#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Cond: Return if",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "x == y => return x + y;",

        .expectedAstPrint =
        "stmt::Conditional [@0:0]\n"
        "├─ expr::Binary [@0:0] with op = '=='\n"
        "   ├─ expr::Identifier [@0:0] with path = 'x'\n"
        "   ├─ expr::Identifier [@0:5] with path = 'y'\n"
        "├─ stmt::Return [@0:10]\n"
        "   ├─ expr::Binary [@0:17] with op = '+'\n"
        "      ├─ expr::Identifier [@0:17] with path = 'x'\n"
        "      ├─ expr::Identifier [@0:21] with path = 'y'",

        .expectedPrettyPrint =
        "(x == y) => return (x + y);",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Cond: Then is an expression statement",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "x == y => foo.Bar{sum: x+y, product: x*y};",

        .expectedAstPrint =
        "stmt::Conditional [@0:0]\n"
        "├─ expr::Binary [@0:0] with op = '=='\n"
        "   ├─ expr::Identifier [@0:0] with path = 'x'\n"
        "   ├─ expr::Identifier [@0:5] with path = 'y'\n"
        "├─ stmt::Expression [@0:10]\n"
        "   ├─ expr::Record [@0:10] with size = 2\n"
        "      ├─ type::Identifier [@0:10] with (const, fund, path) = (true, false, 'foo.Bar')\n"
        "      ├─ expr::RecordEntry [@0:18] with key = 'sum'\n"
        "         ├─ expr::Binary [@0:23] with op = '+'\n"
        "            ├─ expr::Identifier [@0:23] with path = 'x'\n"
        "            ├─ expr::Identifier [@0:25] with path = 'y'\n"
        "      ├─ expr::RecordEntry [@0:28] with key = 'product'\n"
        "         ├─ expr::Binary [@0:37] with op = '*'\n"
        "            ├─ expr::Identifier [@0:37] with path = 'x'\n"
        "            ├─ expr::Identifier [@0:39] with path = 'y'",

        .expectedPrettyPrint =
        "(x == y) => foo.Bar{sum: (x + y), product: (x * y)};",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Cond: Correct indentation for the 'then' block",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "x == y => {"
        " {(2*x)|> io.println;}\n"
        "}",

        .expectedAstPrint =
        "stmt::Conditional [@0:0]\n"
        "├─ expr::Binary [@0:0] with op = '=='\n"
        "   ├─ expr::Identifier [@0:0] with path = 'x'\n"
        "   ├─ expr::Identifier [@0:5] with path = 'y'\n"
        "├─ stmt::Block [@0:10] with size = 1\n"
        "   ├─ stmt::Block [@0:12] with size = 1\n"
        "      ├─ stmt::Expression [@0:13]\n"
        "         ├─ expr::Binary [@0:13] with op = '|>'\n"
        "            ├─ expr::Tuple [@0:13] with size = 1\n"
        "               ├─ expr::Binary [@0:14] with op = '*'\n"
        "                  ├─ expr::Integer [@0:14] with value = 2\n"
        "                  ├─ expr::Identifier [@0:16] with path = 'x'\n"
        "            ├─ expr::Identifier [@0:21] with path = 'io.println'",

        .expectedPrettyPrint =
        "(x == y) => {\n"
        "    {\n"
        "        (((2 * x)) |> io.println);\n"
        "    }\n"
        "}",
    });
}
