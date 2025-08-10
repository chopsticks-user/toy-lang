#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Decl: Missing initializing expression",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "x=;",

        .expectedAstPrint =
        "stmt::Decl [@0:0]\n"
        "├─ decl::Identifier [@0:0] with name = 'x'\n"
        "   ├─ (empty)\n"
        "├─ (required)",

        .expectedPrettyPrint =
        "x: {?} = {!};",

        .expectedErrors = {
            {.context = Context::DeclStmt, .reason = Reason::MissingExpr},
        },
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Decl: Identifier",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "x = foo.Bar{x: 0, y: true};",

        .expectedAstPrint =
        "stmt::Decl [@0:0]\n"
        "├─ decl::Identifier [@0:0] with name = 'x'\n"
        "   ├─ (empty)\n"
        "├─ expr::Record [@0:4] with size = 2\n"
        "   ├─ type::Identifier [@0:4] with (const, fund, path) = (true, false, 'foo.Bar')\n"
        "   ├─ expr::RecordEntry [@0:12] with key = 'x'\n"
        "      ├─ expr::Integer [@0:15] with value = 0\n"
        "   ├─ expr::RecordEntry [@0:18] with key = 'y'\n"
        "      ├─ expr::Boolean [@0:21] with value = true",

        .expectedPrettyPrint =
        "x: {?} = foo.Bar{x: 0, y: true};",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Decl: Tuple",
    "[Unit][Parse][Stmt]"
) {
    assertStmt({
        .source =
        "(x, y, z: Bool) = (0, 0.0, false);",

        .expectedAstPrint =
        "stmt::Decl [@0:0]\n"
        "├─ decl::Tuple [@0:0] with size = 3\n"
        "   ├─ decl::Identifier [@0:1] with name = 'x'\n"
        "      ├─ (empty)\n"
        "   ├─ decl::Identifier [@0:4] with name = 'y'\n"
        "      ├─ (empty)\n"
        "   ├─ decl::Identifier [@0:7] with name = 'z'\n"
        "      ├─ type::Identifier [@0:10] with (const, fund, path) = (true, true, 'Bool')\n"
        "├─ expr::Tuple [@0:18] with size = 3\n"
        "   ├─ expr::Integer [@0:19] with value = 0\n"
        "   ├─ expr::Float [@0:22] with value = 0\n"
        "   ├─ expr::Boolean [@0:27] with value = false",

        .expectedPrettyPrint =
        "(x: {?}, y: {?}, z: Bool) = (0, 0, false);",
    });
}
