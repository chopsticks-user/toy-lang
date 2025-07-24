#include <catch2/catch_test_macros.hpp>

#include "parse/parse.hpp"

class ParseTestFixture {
protected:
    auto parse(tlc::Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_ast = tlc::parse::Parser::operator()(
            tlc::lex::Lexer::operator()(std::move(iss))
        );
    }

    template <tlc::syntax::IsASTNode T>
    static auto parseExpr(tlc::Str source) -> T {
        std::istringstream iss;
        iss.str(std::move(source));
        auto const result = tlc::parse::Parser{
            tlc::lex::Lexer::operator()(std::move(iss))
        }.parseExpr();

        REQUIRE(result.has_value());
        return cast<T>(*result);
    }

    template <tlc::syntax::IsASTNode T>
    static auto cast(tlc::syntax::Node const& node) -> T {
        REQUIRE(tlc::syntax::matchAstType<T>(node));
        return tlc::syntax::astCast<T>(node);
    }

    static auto assertIdentifier(
        tlc::syntax::Node const& node,
        tlc::token::EToken type, tlc::Str const& path
    ) -> void {
        auto const identifier = cast<tlc::syntax::expr::Identifier>(node);
        REQUIRE(identifier.type() == type);
        REQUIRE(identifier.path() == path);
    }

    static auto assertInteger(
        tlc::syntax::Node const& node, tlc::i64 value
    ) -> void {
        auto const integer = cast<tlc::syntax::expr::Integer>(node);
        REQUIRE(integer.value() == value);
    }

    static auto assertFloat(
        tlc::syntax::Node const& node, tlc::f64 value
    ) -> void {
        auto const f = cast<tlc::syntax::expr::Float>(node);
        REQUIRE(f.value() == value);
    }

private:
    tlc::parse::Parser::ParseResult m_ast;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("Parse: Integers", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Floats", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Booleans", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Strings", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Identifiers", "[Parse]") {
    // SECTION("Local identifier") {
    //     auto const id =
    //         parseExpr<tlc::syntax::expr::Identifier>("baz");
    //     assertIdentifier(
    //         id, tlc::token::EToken::Identifier, "baz"
    //     );
    // }

    SECTION("Imported identifier") {
        auto const id =
            parseExpr<tlc::syntax::expr::Identifier>("foo::bar");
        assertIdentifier(
            id, tlc::token::EToken::Identifier, "foo::bar"
        );
    }

    SECTION("Imported type") {
        auto const id =
            parseExpr<tlc::syntax::expr::Identifier>("foo::bar::Baz");
        assertIdentifier(
            id, tlc::token::EToken::UserDefinedType, "foo::bar::Baz"
        );
    }
}

TEST_CASE_WITH_FIXTURE("Parse: Tuples", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Arrays", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Access expressions", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Function applications", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Subscript expressions", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Prefix expressions", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Binary expressions", "[Parse]") {
    SECTION("Simple") {
        auto const expr =
            parseExpr<tlc::syntax::expr::Binary>("-x+3");
        REQUIRE(expr.op() == tlc::token::EToken::Plus);

        auto const left =
            cast<tlc::syntax::expr::Unary>(expr.left());
        REQUIRE(left.op() == tlc::token::EToken::Minus);
        assertIdentifier(
            left.operand(), tlc::token::EToken::Identifier, "x"
        );

        assertInteger(expr.right(), 3);
    }

    SECTION("Prioritize right") {
        auto const expr =
            parseExpr<tlc::syntax::expr::Binary>("-x+y*z");
        REQUIRE(expr.op() == tlc::token::EToken::Plus);

        auto const left =
            cast<tlc::syntax::expr::Unary>(expr.left());
        REQUIRE(left.op() == tlc::token::EToken::Minus);
        assertIdentifier(
            left.operand(), tlc::token::EToken::Identifier, "x"
        );

        auto const right =
            cast<tlc::syntax::expr::Binary>(expr.right());
        REQUIRE(right.op() == tlc::token::EToken::Star);
        assertIdentifier(
            right.left(), tlc::token::EToken::Identifier, "y"
        );
        assertIdentifier(
            right.right(), tlc::token::EToken::Identifier, "z"
        );
    }

    SECTION("Prioritize left") {
        // auto const expr =
        //     parseExpr<tlc::syntax::expr::Binary>("x*-y+z");
        // REQUIRE(expr.op() == tlc::token::EToken::Plus);
        //
        // auto const left =
        //     cast<tlc::syntax::expr::Binary>(expr.left());
        // REQUIRE(left.op() == tlc::token::EToken::Star);
        // assertIdentifier(
        //     left.left(), tlc::token::EToken::Identifier, "x"
        // );
        // auto const leftRight =
        //     cast<tlc::syntax::expr::Unary>(left.right());
        // REQUIRE(leftRight.op() == tlc::token::EToken::Minus);
        // assertIdentifier(
        //     leftRight.operand(),
        //     tlc::token::EToken::Identifier, "y"
        // );
        //
        // assertIdentifier(
        //     expr.right(), tlc::token::EToken::Identifier, "z"
        // );
    }
}

TEST_CASE_WITH_FIXTURE("Parse: Ternary expressions", "[Parse]") {}
