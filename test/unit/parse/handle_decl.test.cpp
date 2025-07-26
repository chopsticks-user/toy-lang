#include "parse.test.hpp"

using tlc::token::EToken;
using namespace tlc::syntax;

TEST_CASE_WITH_FIXTURE("Parse: Type decl", "[Parse]") {
    // AssertType::identifier(
    //     "Int", {.fundamental = true, .imported = false, .path = "Int"}
    // );
    //
    // AssertType::identifier(
    //     "foo::bar::Baz", {
    //         .fundamental = false, .imported = true, .path = "foo::bar::Baz"
    //     }
    // );
}
