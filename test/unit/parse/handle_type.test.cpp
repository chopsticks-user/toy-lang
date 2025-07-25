#include "parse.test.hpp"

using tlc::token::EToken;
using namespace tlc::syntax;

#define GENERATE_COMPARE_ASSERTION(field) \
    info.field.transform([&](auto value) { \
        REQUIRE(id.field() == value); \
        return ""; \
    })

#define GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(name) \
    auto ParseTestFixture::AssertType::name( \
        tlc::Str source, IdentifierInfo info \
    ) -> void { \
        return (name)( \
            parseType<type::Identifier>(std::move(source)), std::move(info) \
        ); \
    }

auto ParseTestFixture::AssertType::identifier(
    Node const& node, IdentifierInfo info
) -> void {
    auto const& id = cast<type::Identifier>(node);
    GENERATE_COMPARE_ASSERTION(fundamental);
    GENERATE_COMPARE_ASSERTION(imported);
    GENERATE_COMPARE_ASSERTION(path);
}

GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(identifier);

TEST_CASE_WITH_FIXTURE("Parse: Type", "[Parse]") {
    AssertType::identifier(
        "Int", {
            .fundamental = true,
            .imported = false,
            .path = "Int"
        }
    );

    AssertType::identifier(
        "foo::bar::Baz", {
            .fundamental = false,
            .imported = true,
            .path = "foo::bar::Baz"
        }
    );
}
