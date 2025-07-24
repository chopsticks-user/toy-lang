#include <catch2/catch_test_macros.hpp>

#include "parse/parse.hpp"

using tlc::token::EToken;
using namespace tlc::syntax;

class ParseTestFixture {
protected:
    auto parse(tlc::Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_ast = tlc::parse::Parser::operator()(
            tlc::lex::Lexer::operator()(std::move(iss))
        );
    }

    template <IsASTNode T>
    static auto parseExpr(tlc::Str source) -> T {
        std::istringstream iss;
        iss.str(std::move(source));
        auto const result = tlc::parse::Parser{
            tlc::lex::Lexer::operator()(std::move(iss))
        }.parseExpr();

        REQUIRE(result.has_value());
        return cast<T>(*result);
    }

    template <IsASTNode T>
    static auto cast(Node const& node) -> T {
        REQUIRE(matchAstType<T>(node));
        return astCast<T>(node);
    }

    static auto assertIdentifier(
        Node const& node,
        EToken type, tlc::Str const& path
    ) -> void {
        auto const identifier = cast<expr::Identifier>(node);
        REQUIRE(identifier.type() == type);
        REQUIRE(identifier.path() == path);
    }

    static auto assertIdentifier(
        tlc::Str source,
        EToken const type, tlc::Str const& path
    ) -> void {
        return assertIdentifier(
            parseExpr<expr::Identifier>(std::move(source)),
            type, path
        );
    }

    static auto assertInteger(
        Node const& node,
        tlc::i64 const value
    ) -> void {
        auto const integer = cast<expr::Integer>(node);
        REQUIRE(integer.value() == value);
    }

    static auto assertInteger(
        tlc::Str source,
        tlc::i64 const value
    ) -> void {
        return assertInteger(
            parseExpr<expr::Integer>(std::move(source)),
            value
        );
    }

    static auto assertFloat(
        Node const& node, tlc::f64 const value
    ) -> void {
        auto const f = cast<expr::Float>(node);
        REQUIRE(f.value() == value);
    }

    static auto assertFloat(
        tlc::Str source,
        tlc::f64 const value
    ) -> void {
        return assertFloat(
            parseExpr<expr::Float>(std::move(source)),
            value
        );
    }

private:
    tlc::parse::Parser::ParseResult m_ast;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("Parse: Integers", "[Parse]") {
    SECTION("Base 10") {
        assertInteger("31415", 31415);
        assertInteger("0", 0);
    }

    SECTION("Base 16") {
        assertInteger("0x123456789abcdef", 0x123456789abcdef);
        assertInteger("0x0", 0);
    }

    SECTION("Base 8") {
        assertInteger("01234567", 01234567);
        assertInteger("00", 0);
    }

    SECTION("Base 2") {
        assertInteger("0b1010101001", 0b1010101001);
        assertInteger("0b0", 0);
    }
}

TEST_CASE_WITH_FIXTURE("Parse: Floats", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Booleans", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Strings", "[Parse]") {}

TEST_CASE_WITH_FIXTURE("Parse: Identifiers", "[Parse]") {
    SECTION("Local identifier") {
        assertIdentifier(
            "baz", EToken::Identifier, "baz"
        );
    }

    SECTION("Imported identifier") {
        assertIdentifier(
            "foo::bar", EToken::Identifier, "foo::bar"
        );
    }

    SECTION("Imported type") {
        assertIdentifier(
            "foo::bar::Baz", EToken::UserDefinedType, "foo::bar::Baz"
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
            parseExpr<expr::Binary>("-x+3");
        REQUIRE(expr.op() == EToken::Plus);

        auto const left =
            cast<expr::Unary>(expr.left());
        REQUIRE(left.op() == EToken::Minus);
        assertIdentifier(
            left.operand(), EToken::Identifier, "x"
        );

        assertInteger(expr.right(), 3);
    }

    SECTION("Prioritize right") {
        auto const expr =
            parseExpr<expr::Binary>("-x+y*z");
        REQUIRE(expr.op() == EToken::Plus);

        auto const left =
            cast<expr::Unary>(expr.left());
        REQUIRE(left.op() == EToken::Minus);
        assertIdentifier(
            left.operand(), EToken::Identifier, "x"
        );

        auto const right =
            cast<expr::Binary>(expr.right());
        REQUIRE(right.op() == EToken::Star);
        assertIdentifier(
            right.left(), EToken::Identifier, "y"
        );
        assertIdentifier(
            right.right(), EToken::Identifier, "z"
        );
    }

    SECTION("Prioritize left") {
        auto const expr =
            parseExpr<expr::Binary>("x*-y+z");
        REQUIRE(expr.op() == EToken::Plus);

        auto const left =
            cast<expr::Binary>(expr.left());
        REQUIRE(left.op() == EToken::Star);
        assertIdentifier(
            left.left(), EToken::Identifier, "x"
        );
        auto const leftRight =
            cast<expr::Unary>(left.right());
        REQUIRE(leftRight.op() == EToken::Minus);
        assertIdentifier(
            leftRight.operand(),
            EToken::Identifier, "y"
        );

        assertIdentifier(
            expr.right(), EToken::Identifier, "z"
        );
    }
}

TEST_CASE_WITH_FIXTURE("Parse: Ternary expressions", "[Parse]") {}
