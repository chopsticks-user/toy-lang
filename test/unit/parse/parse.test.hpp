#ifndef TLC_TEST_UNIT_PARSE_TEST_HPP
#define TLC_TEST_UNIT_PARSE_TEST_HPP

#include <catch2/catch_test_macros.hpp>

#include "parse/parse.hpp"

class ParseTestFixture {
protected:
    using FnNode = void (*)(tlc::syntax::Node const&);
    using FnNodes = void (*)(tlc::Span<tlc::syntax::Node const>);
    template <tlc::syntax::IsASTNode T>
    using FnNodeT = void (*)(T const&);

    inline static const tlc::fs::path filepath =
        "toy-lang/test/unit/parse.toy";

protected:
    // auto parse(tlc::Str source) -> void {
    //     std::istringstream iss;
    //     iss.str(std::move(source));
    //     m_ast = tlc::parse::Parser::operator()(
    //         tlc::lex::Lexer::operator()(std::move(iss))
    //     );
    // }

    template <tlc::syntax::IsASTNode T>
    static auto parseExpr(tlc::Str source) -> T {
        CAPTURE(source.c_str());
        std::istringstream iss;
        iss.str(std::move(source));
        auto const result = tlc::parse::Parse{
            filepath, tlc::lex::Lex::operator()(std::move(iss))
        }.parseExpr();

        REQUIRE(result.has_value());
        return cast<T>(*result);
    }

    template <tlc::syntax::IsASTNode T>
    static auto parseType(tlc::Str source) -> T {
        std::istringstream iss;
        iss.str(std::move(source));
        auto const result = tlc::parse::Parse{
            filepath, tlc::lex::Lex::operator()(std::move(iss))
        }.parseType();

        REQUIRE(result.has_value());
        return cast<T>(*result);
    }

    template <tlc::syntax::IsASTNode T>
    static auto cast(tlc::syntax::Node const& node) -> T {
        REQUIRE(tlc::syntax::matchAstType<T>(node));
        return tlc::syntax::astCast<T>(node);
    }

    // struct AssertExpr {
    //
    // };

    static auto assertIdentifier(
        tlc::syntax::Node const& node, tlc::Str const& path
    ) -> void;

    static auto assertIdentifier(
        tlc::Str source, tlc::Str const& path
    ) -> void;

    static auto assertInteger(
        tlc::syntax::Node const& node, tlc::i64 value
    ) -> void;

    static auto assertInteger(
        tlc::Str source, tlc::i64 value
    ) -> void;

    static auto assertFloat(
        tlc::syntax::Node const& node, tlc::f64 value
    ) -> void;

    static auto assertFloat(
        tlc::Str source, tlc::f64 value
    ) -> void;

    static auto assertBoolean(
        tlc::syntax::Node const& node, tlc::f64 value
    ) -> void;

    static auto assertBoolean(
        tlc::Str source, tlc::b8 value
    ) -> void;

    static auto assertTuple(
        tlc::syntax::Node const& node, tlc::szt size,
        tlc::Opt<FnNodes> fn = {}
    ) -> void;

    static auto assertTuple(
        tlc::Str source, tlc::szt size,
        tlc::Opt<FnNodes> fn = {}
    ) -> void;

    static auto assertArray(
        tlc::syntax::Node const& node, tlc::szt size,
        tlc::Opt<FnNodes> fn = {}
    ) -> void;

    static auto assertArray(
        tlc::Str source, tlc::szt size,
        tlc::Opt<FnNodes> fn = {}
    ) -> void;

    static auto assertAccessExpr(
        tlc::syntax::Node const& node, tlc::Opt<FnNode> fnObj = {},
        tlc::Opt<FnNode> fnField = {}
    ) -> void;

    static auto assertAccessExpr(
        tlc::Str source, tlc::Opt<FnNode> fnObj = {},
        tlc::Opt<FnNode> fnField = {}
    ) -> void;

    static auto assertFnAppExpr(
        tlc::syntax::Node const& node, tlc::Opt<FnNode> fnCallee = {},
        tlc::Opt<FnNode> fnArgs = {}
    ) -> void;

    static auto assertFnAppExpr(
        tlc::Str source, tlc::Opt<FnNode> fnCallee = {},
        tlc::Opt<FnNode> fnArgs = {}
    ) -> void;

    static auto assertSubscriptExpr(
        tlc::syntax::Node const& node, tlc::Opt<FnNode> fnColl = {},
        tlc::Opt<FnNode> fnSubs = {}
    ) -> void;

    static auto assertSubscriptExpr(
        tlc::Str source, tlc::Opt<FnNode> fnColl = {},
        tlc::Opt<FnNode> fnSubs = {}
    ) -> void;

    static auto assertPrefixExpr(
        tlc::syntax::Node const& node, tlc::token::EToken op,
        tlc::Opt<FnNode> fnOperand = {}
    ) -> void;

    static auto assertPrefixExpr(
        tlc::Str source, tlc::token::EToken op,
        tlc::Opt<FnNode> fnOperand = {}
    ) -> void;

    // todo: assertBinaryExpr

    struct AssertType {
        struct IdentifierInfo {
            tlc::Opt<tlc::b8> fundamental;
            tlc::Opt<tlc::b8> imported;
            tlc::Opt<tlc::Str> path;
        };

        static auto identifier(tlc::syntax::Node const& node, IdentifierInfo info) -> void;
        static auto identifier(tlc::Str source, IdentifierInfo info) -> void;

        struct TupleInfo {
            tlc::Opt<tlc::szt> size;
            tlc::Opt<FnNodes> assert_children;
        };

        static auto tuple(tlc::syntax::Node const& node, TupleInfo info) -> void;
        static auto tuple(tlc::Str source, TupleInfo info) -> void;

        struct InferInfo {
            tlc::Opt<FnNode> assert_expr;
        };

        static auto infer(tlc::syntax::Node const& node, InferInfo info) -> void;
        static auto infer(tlc::Str source, InferInfo info) -> void;

        struct ArrayInfo {
            tlc::Opt<tlc::szt> dim;
            tlc::Opt<FnNode> assert_type;
            tlc::Opt<FnNodeT<tlc::syntax::type::Array>> assert_self;
        };

        static auto array(tlc::syntax::Node const& node, ArrayInfo info) -> void;
        static auto array(tlc::Str source, ArrayInfo info) -> void;

        struct FunctionInfo {
            tlc::Opt<FnNode> assert_args;
            tlc::Opt<FnNode> assert_result;
        };

        static auto function(tlc::syntax::Node const& node, FunctionInfo info) -> void;
        static auto function(tlc::Str source, FunctionInfo info) -> void;
    };

private:
    // tlc::parse::Parser::ParseResult m_ast;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

#endif // TLC_TEST_UNIT_PARSE_TEST_HPP
