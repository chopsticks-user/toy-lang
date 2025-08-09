#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Loop: Single-value iterator",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "for e in r0 {x+=e;}",

        .expectedAstPrint =
        "stmt::Loop [@0:0]\n"
        "├─ decl::Identifier [@0:4] with name = 'e'\n"
        "   ├─ (empty)\n"
        "├─ expr::Identifier [@0:9] with path = 'r0'\n"
        "├─ stmt::Block [@0:12] with size = 1\n"
        "   ├─ stmt::Assign [@0:13] with op = '+='\n"
        "      ├─ expr::Identifier [@0:13] with path = 'x'\n"
        "      ├─ expr::Identifier [@0:16] with path = 'e'",

        .expectedPrettyPrint =
        "for e: {?} in r0 {\n"
        "    x += e;\n"
        "}",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Loop: Multiple-value iterator",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "for (i: Int, v: Float) in r0 {}",

        .expectedAstPrint =
        "stmt::Loop [@0:0]\n"
        "├─ decl::Tuple [@0:4] with size = 2\n"
        "   ├─ decl::Identifier [@0:5] with name = 'i'\n"
        "      ├─ type::Identifier [@0:8] with (const, fund, path) = (true, true, 'Int')\n"
        "   ├─ decl::Identifier [@0:13] with name = 'v'\n"
        "      ├─ type::Identifier [@0:16] with (const, fund, path) = (true, true, 'Float')\n"
        "├─ expr::Identifier [@0:26] with path = 'r0'\n"
        "├─ stmt::Block [@0:29] with size = 0",

        .expectedPrettyPrint =
        "for (i: Int, v: Float) in r0 {}",
    });
}
