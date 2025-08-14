#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Tuple: Empty",
    "[Unit][Parse][Decl]"
) {
    assertDecl({
        .source =
        "()",

        .expectedAstPrint =
        "decl::Tuple [@0:0] with size = 0",

        .expectedPrettyPrint =
        "()",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Tuple: 3 declarations",
    "[Unit][Parse][Decl]"
) {
    assertDecl({
        .source =
        "(x: Int, y: $Float, z)",

        .expectedAstPrint =
        "decl::Tuple [@0:0] with size = 3\n"
        "├─ decl::Identifier [@0:1] with name = 'x'\n"
        "   ├─ type::Identifier [@0:4] with (const, fund, path) = (true, true, 'Int')\n"
        "├─ decl::Identifier [@0:9] with name = 'y'\n"
        "   ├─ type::Identifier [@0:12] with (const, fund, path) = (false, true, 'Float')\n"
        "├─ decl::Identifier [@0:20] with name = 'z'\n"
        "   ├─ (empty)",

        .expectedPrettyPrint =
        "(x: Int, y: $Float, z: {?})",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Tuple: Nested",
    "[Unit][Parse][Decl]"
) {
    // assertDecl({
    //     .source =
    //     "(x: Int, y: $Float, (z, t))",
    //
    //     .expectedAstPrint =
    //     "decl::Tuple [@0:0] with size = 3\n"
    //     "├─ decl::Identifier [@0:1] with name = 'x'\n"
    //     "   ├─ type::Identifier [@0:4] with (const, fund, path) = (true, true, 'Int')\n"
    //     "├─ decl::Identifier [@0:9] with name = 'y'\n"
    //     "   ├─ type::Identifier [@0:12] with (const, fund, path) = (false, true, 'Float')\n"
    //     "├─ decl::Tuple [@0:20] with size = 2\n"
    //     "   ├─ decl::Identifier [@0:21] with name = 'z'\n"
    //     "      ├─ (empty)\n"
    //     "   ├─ decl::Identifier [@0:24] with name = 't'\n"
    //     "      ├─ (empty)",
    //
    //     .expectedPrettyPrint =
    //     "(x: Int, y: $Float, (z: {?}, t: {?}))",
    // });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Tuple: Synchronize until next ','",
    "[Unit][Parse][Decl]"
) {
    assertDecl({
        .source =
        "(x, : Int, z: Float)",

        .expectedAstPrint =
        "decl::Tuple [@0:0] with size = 3\n"
        "├─ decl::Identifier [@0:1] with name = 'x'\n"
        "   ├─ (empty)\n"
        "├─ (required)\n"
        "├─ decl::Identifier [@0:11] with name = 'z'\n"
        "   ├─ type::Identifier [@0:14] with (const, fund, path) = (true, true, 'Float')",

        .expectedPrettyPrint =
        "(x: {?}, {!}, z: Float)",

        .expectedErrors = {
            {.context = Context::TupleDecl, .reason = Reason::MissingDecl},
        },
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Tuple: Synchronize until next ')'",
    "[Unit][Parse][Decl]"
) {
    assertDecl({
        .source =
        "(Int)",

        .expectedAstPrint =
        "decl::Tuple [@0:0] with size = 1\n"
        "├─ (required)",

        .expectedPrettyPrint =
        "({!})",

        .expectedErrors = {
            {.context = Context::TupleDecl, .reason = Reason::MissingDecl},
        },
    });
}
