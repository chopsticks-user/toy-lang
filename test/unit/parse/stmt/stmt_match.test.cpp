#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Match: Simple use case",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "match x {\n"
        "    5 => y := x * 5;\n"
        "    when x < 5 => {}\n"
        "    _ => {}\n"
        "}",

        .expectedAstPrint =
        "stmt::Match [@0:0]\n"
        "├─ expr::Identifier [@0:6] with path = 'x'\n"
        "├─ stmt::MatchCase [@1:4]\n"
        "   ├─ expr::Integer [@1:4] with value = 5\n"
        "   ├─ (empty)\n"
        "   ├─ stmt::Assign [@1:9] with op = ':='\n"
        "      ├─ expr::Identifier [@1:9] with path = 'y'\n"
        "      ├─ expr::Binary [@1:14] with op = '*'\n"
        "         ├─ expr::Identifier [@1:14] with path = 'x'\n"
        "         ├─ expr::Integer [@1:18] with value = 5\n"
        "├─ stmt::MatchCase [@2:4]\n"
        "   ├─ (empty)\n"
        "   ├─ expr::Binary [@2:9] with op = '<'\n"
        "      ├─ expr::Identifier [@2:9] with path = 'x'\n"
        "      ├─ expr::Integer [@2:13] with value = 5\n"
        "   ├─ stmt::Block [@2:18] with size = 0\n"
        "├─ stmt::Block [@3:9] with size = 0",

        .expectedPrettyPrint =
        "match x {\n"
        "    5 when {?} => y := (x * 5);\n"
        "    {?} when (x < 5) => {\n"
        "}\n"
        "    _ => {\n"
        "}\n"
        "}",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Match: With anonymous identifiers",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "match (x,y) {\n"
        "    (_, 5) => z := x + y;\n"
        "    when x < 5 && y > 0 => {}\n"
        "    (10, _) when y <= 0 => {}\n"
        "    _ => {}\n"
        "}",

        .expectedAstPrint =
        "stmt::Match [@0:0]\n"
        "├─ expr::Tuple [@0:6] with size = 2\n"
        "   ├─ expr::Identifier [@0:7] with path = 'x'\n"
        "   ├─ expr::Identifier [@0:9] with path = 'y'\n"
        "├─ stmt::MatchCase [@1:4]\n"
        "   ├─ expr::Tuple [@1:4] with size = 2\n"
        "      ├─ expr::Identifier [@1:5] with path = '_'\n"
        "      ├─ expr::Integer [@1:8] with value = 5\n"
        "   ├─ (empty)\n"
        "   ├─ stmt::Assign [@1:14] with op = ':='\n"
        "      ├─ expr::Identifier [@1:14] with path = 'z'\n"
        "      ├─ expr::Binary [@1:19] with op = '+'\n"
        "         ├─ expr::Identifier [@1:19] with path = 'x'\n"
        "         ├─ expr::Identifier [@1:23] with path = 'y'\n"
        "├─ stmt::MatchCase [@2:4]\n"
        "   ├─ (empty)\n"
        "   ├─ expr::Binary [@2:9] with op = '&&'\n"
        "      ├─ expr::Binary [@2:9] with op = '<'\n"
        "         ├─ expr::Identifier [@2:9] with path = 'x'\n"
        "         ├─ expr::Integer [@2:13] with value = 5\n"
        "      ├─ expr::Binary [@2:18] with op = '>'\n"
        "         ├─ expr::Identifier [@2:18] with path = 'y'\n"
        "         ├─ expr::Integer [@2:22] with value = 0\n"
        "   ├─ stmt::Block [@2:27] with size = 0\n"
        "├─ stmt::MatchCase [@3:4]\n"
        "   ├─ expr::Tuple [@3:4] with size = 2\n"
        "      ├─ expr::Integer [@3:5] with value = 10\n"
        "      ├─ expr::Identifier [@3:9] with path = '_'\n"
        "   ├─ expr::Binary [@3:17] with op = '<='\n"
        "      ├─ expr::Identifier [@3:17] with path = 'y'\n"
        "      ├─ expr::Integer [@3:22] with value = 0\n"
        "   ├─ stmt::Block [@3:27] with size = 0\n"
        "├─ stmt::Block [@4:9] with size = 0",

        .expectedPrettyPrint =
        "match (x, y) {\n"
        "    (_, 5) when {?} => z := (x + y);\n"
        "    {?} when ((x < 5) && (y > 0)) => {\n"
        "}\n"
        "    (10, _) when (y <= 0) => {\n"
        "}\n"
        "    _ => {\n"
        "}\n"
        "}",
    });
}
