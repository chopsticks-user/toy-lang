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
