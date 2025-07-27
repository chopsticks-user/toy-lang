#include "parse.test.hpp"


using tlc::token::EToken;
using namespace tlc::syntax;

const auto let_stmt = R"(
    // basic
    let x = 0;
    let x: Int = 0;
    let $x: Int = 0;
    let $x = 0;

    // arrays
    let x = [];
    let x: Int[] = [];
    let x = [0, 1, 2];

    // objects
    let x = Foo{name: "", data: []};
    let x: Foo = {};

    // tuples
    let (x, $y: Float[], z: Foo) = (5, [], {name: "", data: []});
)";

auto fn(auto const location = std::source_location::current()) {
    return location;
}

TEST_CASE_WITH_FIXTURE("Parse: Type decl", "[Parse]") {}
