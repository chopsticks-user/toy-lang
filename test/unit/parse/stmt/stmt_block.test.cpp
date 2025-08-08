#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Block: Empty",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "{  }",

        .expectedAstPrint =
        "stmt::Block [@0:0] with size = 0",

        .expectedPrettyPrint =
        "{\n"
        "}",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Block: Simple block",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "{\n"
        "    x = 3.14159;\n"
        "    x := x + y;\n"
        "}",

        .expectedAstPrint =
        "stmt::Block [@0:0] with size = 2\n"
        "├─ stmt::Decl [@1:4]\n"
        "   ├─ decl::Identifier [@1:4] with name = 'x'\n"
        "      ├─ (empty)\n"
        "   ├─ expr::Float [@1:8] with value = 3.14159\n"
        "├─ stmt::Assign [@2:4] with op = ':='\n"
        "   ├─ expr::Identifier [@2:4] with path = 'x'\n"
        "   ├─ expr::Binary [@2:9] with op = '+'\n"
        "      ├─ expr::Identifier [@2:9] with path = 'x'\n"
        "      ├─ expr::Identifier [@2:13] with path = 'y'",

        .expectedPrettyPrint =
        "{\n"
        "    x: {?} = 3.14159;\n"
        "    x := (x + y);\n"
        "}",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Block: Nested",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "{\n"
        "{{}{}}}",

        .expectedAstPrint =
        "stmt::Block [@0:0] with size = 1\n"
        "├─ stmt::Block [@1:0] with size = 2\n"
        "   ├─ stmt::Block [@1:1] with size = 0\n"
        "   ├─ stmt::Block [@1:3] with size = 0",

        // .expectedPrettyPrint =
        // "{\n"
        // "    {\n"
        // "        {\n"
        // "        }\n"
        // "        {\n"
        // "        }\n"
        // "    }\n"
        // "}",
    });
}
