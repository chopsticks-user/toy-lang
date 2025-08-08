#include "parse.test.hpp"

TEST_CASE_WITH_FIXTURE(
    "Parse.Global.Trait: Empty",
    "[Unit][Parse][Global]"
) {
    // assertTranslationUnit({
    //     .source =
    //     "for e in r0 {}",
    //
    //     .expectedAstPrint =
    //     "stmt::Loop [@0:0]\n"
    //     "├─ decl::Identifier [@0:4] with name = 'e'\n"
    //     "   ├─ (empty)\n"
    //     "├─ expr::Identifier [@0:9] with path = 'r0'\n"
    //     "├─ stmt::Block [@0:12] with size = 0",
    //
    //     .expectedPrettyPrint =
    //     "for e: {?} in r0 {\n"
    //     "}",
    // });
}
