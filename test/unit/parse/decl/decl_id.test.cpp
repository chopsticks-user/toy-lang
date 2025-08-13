#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Identifier: Single variable",
    "[Unit][Parse][Decl]"
) {
    assertDecl({
        .source =
        "x: $Foo",

        .expectedAstPrint =
        "decl::Identifier [@0:0] with name = 'x'\n"
        "├─ type::Identifier [@0:3] with (const, fund, path) = (false, false, 'Foo')",

        .expectedPrettyPrint =
        "x: $Foo",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Identifier: Inferred type",
    "[Unit][Parse][Decl]"
) {
    assertDecl({
        .source =
        "x",

        .expectedAstPrint =
        "decl::Identifier [@0:0] with name = 'x'\n"
        "├─ (empty)",

        .expectedPrettyPrint =
        "x: {?}",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Identifier: Anonymous",
    "[Unit][Parse][Decl]"
) {
    assertDecl({
        .source =
        "_",

        .expectedAstPrint =
        "decl::Identifier [@0:0] with name = '_'\n"
        "├─ (empty)",

        .expectedPrettyPrint =
        "_: {?}",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Identifier: Anonymous with type",
    "[Unit][Parse][Decl]"
) {
    assertDecl({
        .source =
        "_: Int",

        .expectedAstPrint =
        "decl::Identifier [@0:0] with name = '_'\n"
        "├─ type::Identifier [@0:3] with (const, fund, path) = (true, true, 'Int')",

        .expectedPrettyPrint =
        "_: Int",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Identifier: Colon without preceding identifier",
    "[Unit][Parse][Decl]"
) {
    // assertDecl({
    //     .source =
    //     ": Int",
    //
    //
    //     .expectedErrors = {
    //         {.context = Context::IdDecl, .reason = Reason::MissingType},
    //     },
    // });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Decl.Identifier: Missing type after colon",
    "[Unit][Parse][Decl]"
) {
    assertDecl({
        .source =
        "x: ",

        .expectedAstPrint =
        "decl::Identifier [@0:0] with name = 'x'\n"
        "├─ (required)",

        .expectedPrettyPrint =
        "x: {!}",

        .expectedErrors = {
            {.context = Context::IdDecl, .reason = Reason::MissingType},
        },
    });
}
