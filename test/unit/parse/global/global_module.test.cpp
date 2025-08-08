#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Global.Module: Empty",
    "[Unit][Parse][Global]"
) {
    assertTranslationUnit({
        .source =
        "module;",

        .expectedAstPrint =
        "(required)\n(empty)",

        .expectedPrettyPrint =
        "",

        .expectedErrors = {
            {.context = Context::TranslationUnit, .reason = Reason::MissingDecl},
        },
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Global.Module: 1-level path",
    "[Unit][Parse][Global]"
) {
    assertTranslationUnit({
        .source =
        "module foo;",

        .expectedAstPrint =
        "global::ModuleDecl [@0:0]\n"
        "├─ expr::Identifier [@0:7] with path = 'foo'\n"
        "(empty)",

        .expectedPrettyPrint =
        "module foo;",
    });
}

TEST_CASE_WITH_FIXTURE(
    "Parse.Global.Module: 3-level path",
    "[Unit][Parse][Global]"
) {
    assertTranslationUnit({
        .source =
        "module foo.bar.baz;",

        .expectedAstPrint =
        "global::ModuleDecl [@0:0]\n"
        "├─ expr::Identifier [@0:7] with path = 'foo.bar.baz'\n"
        "(empty)",

        .expectedPrettyPrint =
        "module foo.bar.baz;",
    });
}
