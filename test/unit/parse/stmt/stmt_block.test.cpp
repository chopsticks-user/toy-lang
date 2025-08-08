#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Stmt.Block: Empty",
    "[Unit][Parse][Stmt]"
) {
    // assertTranslationUnit({
    //     .source =
    //     "module;",
    //
    //     .expectedAstPrint =
    //     "(required)\n(empty)",
    //
    //     .expectedPrettyPrint =
    //     "",
    //
    //     .expectedErrors = {
    //         {.context = Context::TranslationUnit, .reason = Reason::MissingDecl},
    //     },
    // });
}
