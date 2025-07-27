#include "parse.test.hpp"

using tlc::token::EToken;
using namespace tlc::syntax;

TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE(
    decl, Decl, identifier, Identifier
) {
    TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(decl::Identifier);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(identifier);
    TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(type);
}

TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(decl, Decl, identifier, Identifier);


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

TEST_CASE_WITH_FIXTURE("Parse: Identifier decl", "[Parse]") {
    AssertDecl::identifier(
        "x: Foo", {
            .constant = true,
            .assert_identifier = [](Node const& id) {
                AssertExpr::identifier(id, {.path = "x"});
            },
            .assert_type = [](Node const& id) {
                AssertType::identifier(
                    id, {
                        .fundamental = false, .imported = false, .path = "Foo"
                    }
                );
            },
        }
    );

    AssertDecl::identifier(
        "$x: Float", {
            .constant = false,
            .assert_identifier = [](Node const& id) {
                AssertExpr::identifier(id, {.path = "x"});
            },
            .assert_type = [](Node const& type) {
                AssertType::identifier(
                    type, {
                        .fundamental = true, .imported = false, .path = "Float"
                    }
                );
            },
        }
    );

    AssertDecl::identifier(
        "$x", {
            .constant = false,
            .assert_identifier = [](Node const& id) {
                AssertExpr::identifier(id, {.path = "x"});
            },
            .assert_type = [](Node const& type) {
                REQUIRE(isEmptyNode(type));
            },
        }
    );

    AssertDecl::identifier(
        "x", {
            .constant = true,
            .assert_identifier = [](Node const& id) {
                AssertExpr::identifier(id, {.path = "x"});
            },
            .assert_type = [](Node const& type) {
                REQUIRE(isEmptyNode(type));
            },
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Tuple decl", "[Parse]") {}
