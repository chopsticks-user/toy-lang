#include "parse.test.hpp"

using tlc::token::EToken;
using namespace tlc::syntax;

#define GENERATE_COMPARE_ASSERTION(field) \
    info.field.transform([&ast](auto value) { \
        REQUIRE(ast.field() == value); \
        return ""; \
    })

#define GENERATE_CHILD_NODE_ASSERTION(accessorName) \
    info.assert_##accessorName.transform([&ast](auto const& fn) { \
        fn(ast.accessorName()); \
        return ""; \
    })

#define GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(lc_name, uc_name) \
    auto ParseTestFixture::AssertType::lc_name( \
        tlc::Str source, uc_name##Info info \
    ) -> void { \
        CAPTURE(source); \
        return (lc_name)( \
            parseType<type::uc_name>(std::move(source)), std::move(info) \
        ); \
    }

auto ParseTestFixture::AssertType::identifier(
    Node const& node, IdentifierInfo info
) -> void {
    auto const& ast = cast<type::Identifier>(node);
    GENERATE_COMPARE_ASSERTION(fundamental);
    GENERATE_COMPARE_ASSERTION(imported);
    GENERATE_COMPARE_ASSERTION(path);
}

auto ParseTestFixture::AssertType::tuple(
    Node const& node, TupleInfo info
) -> void {
    auto const& ast = cast<type::Tuple>(node);
    GENERATE_COMPARE_ASSERTION(size);
    GENERATE_CHILD_NODE_ASSERTION(children);
}

auto ParseTestFixture::AssertType::infer(
    Node const& node, InferInfo info
) -> void {
    auto const& ast = cast<type::Infer>(node);
    GENERATE_CHILD_NODE_ASSERTION(expr);
}

GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(identifier, Identifier);
GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(tuple, Tuple);
GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(infer, Infer);

TEST_CASE_WITH_FIXTURE("Parse: Type identifiers", "[Parse]") {
    AssertType::identifier(
        "Int", {.fundamental = true, .imported = false, .path = "Int"}
    );

    AssertType::identifier(
        "foo::bar::Baz", {
            .fundamental = false, .imported = true, .path = "foo::bar::Baz"
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Type tuples", "[Parse]") {
    AssertType::tuple("()", {.size = 0});

    AssertType::tuple(
        "(Foo)", {
            .size = 1,
            .assert_children = [](tlc::Span<Node const> const nodes) {
                AssertType::identifier(
                    nodes[0],
                    {.fundamental = false, .imported = false, .path = "Foo"}
                );
            }
        }
    );

    AssertType::tuple(
        "(Int, Int)", {
            .size = 2,
            .assert_children = [](tlc::Span<Node const> const nodes) {
                AssertType::identifier(
                    nodes[0],
                    {.fundamental = true, .imported = false, .path = "Int"}
                );
                AssertType::identifier(
                    nodes[1],
                    {.fundamental = true, .imported = false, .path = "Int"}
                );
            }
        }
    );

    AssertType::tuple(
        "(Int, Float, Int, foo::Bar)", {
            .size = 4,
            .assert_children = [](tlc::Span<Node const> const nodes) {
                AssertType::identifier(
                    nodes[0],
                    {.fundamental = true, .imported = false, .path = "Int"}
                );
                AssertType::identifier(
                    nodes[1],
                    {.fundamental = true, .imported = false, .path = "Float"}
                );
                AssertType::identifier(
                    nodes[2],
                    {.fundamental = true, .imported = false, .path = "Int"}
                );
                AssertType::identifier(
                    nodes[3],
                    {.fundamental = false, .imported = true, .path = "foo::Bar"}
                );
            }
        }
    );
}

TEST_CASE_WITH_FIXTURE("Parse: Type inference operator", "[Parse]") {
    AssertType::infer(
        "[[x]]", {
            .assert_expr = [](Node const& id) {
                assertIdentifier(id, EToken::Identifier, "x");
            }
        }
    );

    AssertType::infer(
        "[[3.14159]]", {
            .assert_expr = [](Node const& value) {
                assertFloat(value, 3.14159);
            }
        }
    );
}
