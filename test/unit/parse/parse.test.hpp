#ifndef TLC_TEST_UNIT_PARSE_TEST_HPP
#define TLC_TEST_UNIT_PARSE_TEST_HPP

#include <catch2/catch_test_macros.hpp>

#include "parse/parse.hpp"

class ParseTestFixture {
    using FnNode = void (*)(tlc::syntax::Node const&);
    using FnNodes = void (*)(tlc::Span<tlc::syntax::Node const>);

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

    static auto assertIdentifier(
        tlc::Str source,
        tlc::token::EToken const type, tlc::Str const& path
    ) -> void {
        return assertIdentifier(
            parseExpr<tlc::syntax::expr::Identifier>(std::move(source)),
            type, path
        );
    }

    static auto assertInteger(
        tlc::syntax::Node const& node, tlc::i64 const value
    ) -> void {
        auto const integer = cast<tlc::syntax::expr::Integer>(node);
        REQUIRE(integer.value() == value);
    }

    static auto assertInteger(
        tlc::Str source, tlc::i64 const value
    ) -> void {
        return assertInteger(
            parseExpr<tlc::syntax::expr::Integer>(std::move(source)), value
        );
    }

    static auto assertFloat(
        tlc::syntax::Node const& node, tlc::f64 const value
    ) -> void {
        auto const f = cast<tlc::syntax::expr::Float>(node);
        REQUIRE(f.value() == value);
    }

    static auto assertFloat(
        tlc::Str source, tlc::f64 const value
    ) -> void {
        return assertFloat(
            parseExpr<tlc::syntax::expr::Float>(std::move(source)), value
        );
    }

    static auto assertBoolean(
        tlc::syntax::Node const& node, tlc::f64 const value
    ) -> void {
        auto const f = cast<tlc::syntax::expr::Boolean>(node);
        REQUIRE(f.value() == value);
    }

    static auto assertBoolean(
        tlc::Str source, tlc::b8 const value
    ) -> void {
        return assertBoolean(
            parseExpr<tlc::syntax::expr::Boolean>(std::move(source)), value
        );
    }

    static auto assertTuple(
        tlc::syntax::Node const& node, tlc::szt const size,
        tlc::Opt<FnNodes> const fn = {}
    ) -> void {
        auto const tuple = cast<tlc::syntax::expr::Tuple>(node);
        REQUIRE(tuple.nChildren() == size);
        if (fn) {
            fn.value()(tuple.children());
        }
    }

    static auto assertTuple(
        tlc::Str source, tlc::szt const size,
        tlc::Opt<FnNodes> fn = {}
    ) -> void {
        return assertTuple(
            parseExpr<tlc::syntax::expr::Tuple>(std::move(source)),
            size, std::move(fn)
        );
    }

    static auto assertArray(
        tlc::syntax::Node const& node, tlc::szt const size,
        tlc::Opt<FnNodes> const fn = {}
    ) -> void {
        auto const array = cast<tlc::syntax::expr::Array>(node);
        REQUIRE(array.nChildren() == size);
        if (fn) {
            fn.value()(array.children());
        }
    }

    static auto assertArray(
        tlc::Str source, tlc::szt const size,
        tlc::Opt<FnNodes> fn = {}
    ) -> void {
        return assertArray(
            parseExpr<tlc::syntax::expr::Array>(std::move(source)),
            size, std::move(fn)
        );
    }

    static auto assertAccessExpr(
        tlc::syntax::Node const& node, tlc::Opt<FnNode> const fnObj = {},
        tlc::Opt<FnNode> const fnField = {}
    ) -> void {
        auto const expr = cast<tlc::syntax::expr::Access>(node);
        if (fnObj) {
            fnObj.value()(expr.object());
        }
        if (fnField) {
            fnField.value()(expr.field());
        }
    }

    static auto assertAccessExpr(
        tlc::Str source, tlc::Opt<FnNode> const fnObj = {},
        tlc::Opt<FnNode> fnField = {}
    ) -> void {
        return assertAccessExpr(
            parseExpr<tlc::syntax::expr::Access>(std::move(source)),
            std::move(fnObj), std::move(fnField)
        );
    }

    static auto assertFnAppExpr(
        tlc::syntax::Node const& node, tlc::Opt<FnNode> const fnCallee = {},
        tlc::Opt<FnNode> const fnArgs = {}
    ) -> void {
        auto const expr = cast<tlc::syntax::expr::FnApp>(node);
        if (fnCallee) {
            fnCallee.value()(expr.callee());
        }
        if (fnArgs) {
            fnArgs.value()(expr.args());
        }
    }

    static auto assertFnAppExpr(
        tlc::Str source, tlc::Opt<FnNode> const fnCallee = {},
        tlc::Opt<FnNode> fnArgs = {}
    ) -> void {
        return assertFnAppExpr(
            parseExpr<tlc::syntax::expr::FnApp>(std::move(source)),
            std::move(fnCallee), std::move(fnArgs)
        );
    }

    static auto assertSubscriptExpr(
        tlc::syntax::Node const& node, tlc::Opt<FnNode> const fnColl = {},
        tlc::Opt<FnNode> const fnSubs = {}
    ) -> void {
        auto const expr = cast<tlc::syntax::expr::Subscript>(node);
        if (fnColl) {
            fnColl.value()(expr.collection());
        }
        if (fnSubs) {
            fnSubs.value()(expr.subscript());
        }
    }

    static auto assertSubscriptExpr(
        tlc::Str source, tlc::Opt<FnNode> const fnColl = {},
        tlc::Opt<FnNode> fnSubs = {}
    ) -> void {
        return assertSubscriptExpr(
            parseExpr<tlc::syntax::expr::Subscript>(std::move(source)),
            std::move(fnColl),
            std::move(fnSubs)
        );
    }

    static auto assertPrefixExpr(
        tlc::syntax::Node const& node, tlc::token::EToken op,
        tlc::Opt<FnNode> const fnOperand = {}
    ) -> void {
        auto const expr = cast<tlc::syntax::expr::Prefix>(node);
        REQUIRE(expr.op() == op);
        if (fnOperand) {
            fnOperand.value()(expr.operand());
        }
    }

    static auto assertPrefixExpr(
        tlc::Str source, tlc::token::EToken const op,
        tlc::Opt<FnNode> const fnOperand = {}
    ) -> void {
        return assertPrefixExpr(
            parseExpr<tlc::syntax::expr::Prefix>(std::move(source)),
            op, std::move(fnOperand)
        );
    }

    // todo: assertBinaryExpr

private:
    tlc::parse::Parser::ParseResult m_ast;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

#endif // TLC_TEST_UNIT_PARSE_TEST_HPP
