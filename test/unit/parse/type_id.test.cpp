#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Identifier: Local",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "Baz",

        .expectedAstPrint =
        "type::Identifier [@0:0] with "
        "(const, fund, path) = (true, false, 'Baz')",

        .expectedPrettyPrint =
        "Baz",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Identifier: Imported",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "foo.Bar",

        .expectedAstPrint =
        "type::Identifier [@0:0] with "
        "(const, fund, path) = (true, false, 'foo.Bar')",

        .expectedPrettyPrint =
        "foo.Bar",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Identifier: Nested",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "foo.bar.baz.Boo",

        .expectedAstPrint =
        "type::Identifier [@0:0] with "
        "(const, fund, path) = (true, false, 'foo.bar.baz.Boo')",

        .expectedPrettyPrint =
        "foo.bar.baz.Boo",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Identifier: snake_case is undefined and divided into multiple parts",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "Foo_Bar_Baz",

        .expectedAstPrint =
        "type::Identifier [@0:0] with "
        "(const, fund, path) = (true, false, 'Foo')",

        .expectedPrettyPrint =
        "Foo",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Type.Infer: Of a binary expression",
    "[Unit][Parse][Type]"
) {
    assertType({
        .source =
        "[[foo.bar + 1]]",

        .expectedAstPrint =
        "type::Infer [@0:0]\n"
        "├─ expr::Binary [@0:2] with op = '+'\n"
        "   ├─ expr::Identifier [@0:2] with path = 'foo.bar'\n"
        "   ├─ expr::Integer [@0:12] with value = 1",

        .expectedPrettyPrint =
        "[[ (foo.bar + 1) ]]",
    });
}
