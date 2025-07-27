#ifndef TLC_TEST_UNIT_PARSE_TEST_HPP
#define TLC_TEST_UNIT_PARSE_TEST_HPP

#include <catch2/catch_test_macros.hpp>
#include <source_location>

#include "parse/parse.hpp"

#define TLC_TEST_GENERATE_COMPARE_ASSERTION(field) \
    info.field.transform([&ast](auto value) { \
        REQUIRE(ast.field() == value); \
        return ""; \
    })

#define TLC_TEST_GENERATE_SELF_ASSERTION() \
    info.assert_self.transform([&ast](auto const& fn) { \
        fn(ast); \
        return ""; \
    })

#define TLC_TEST_GENERATE_CHILD_NODE_ASSERTION(accessorName) \
    info.assert_##accessorName.transform([&ast](auto const& fn) { \
        fn(ast.accessorName()); \
        return ""; \
    })

#define TLC_TEST_GENERATE_ASSERT_FROM_SOURCE_OVERLOAD(lc_syntax_ns, uc_syntax_ns, lc_name, uc_name) \
    auto ParseTestFixture::Assert##uc_syntax_ns::lc_name( \
        tlc::Str source, uc_name info, std::source_location location \
    ) -> void { \
        return (lc_name)( \
            parse##uc_syntax_ns<lc_syntax_ns::uc_name>(std::move(source)), \
            std::move(info), std::move(location) \
    ); \
}

#define TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_SETUP(node_type) \
    INFO(std::format("{}:{}", location.file_name(), location.line())); \
    auto const& ast = cast<node_type>(node);

#define TLC_TEST_GENERATE_ASSERT_FROM_NODE_OVERLOAD_PROTOTYPE( \
    lc_syntax_ns, uc_syntax_ns, lc_name, uc_name \
) \
    auto ParseTestFixture::Assert##uc_syntax_ns::lc_name( \
        Node const& node, uc_name info, std::source_location const location \
    ) -> void

#define TLC_TEST_GENERATE_ASSERT_DECL(lc_name, uc_name) \
        static auto lc_name( \
            tlc::syntax::Node const& node, uc_name info, \
            std::source_location location = std::source_location::current() \
        ) -> void; \
        static auto lc_name( \
            tlc::Str source, uc_name info, \
            std::source_location location = std::source_location::current() \
        ) -> void;

#define TLC_TEST_GENERATE_PARSE_OVERLOAD(uc_syntax_ns) \
    template <tlc::syntax::IsASTNode T> \
    static auto parse##uc_syntax_ns( \
        tlc::Str source, \
        std::source_location const location = std::source_location::current() \
    ) -> T { \
        INFO(std::format("{}:{}", location.file_name(), location.line())); \
        std::istringstream iss; \
        iss.str(std::move(source)); \
        auto const result = tlc::parse::Parse{ \
            filepath, tlc::lex::Lex::operator()(std::move(iss)) \
        }.parse##uc_syntax_ns(); \
        REQUIRE(result.has_value()); \
        return cast<T>(*result); \
    }

class ParseTestFixture {
protected:
    using FnNode = void (*)(tlc::syntax::Node const&);
    using FnNodes = void (*)(tlc::Span<tlc::syntax::Node const>);
    template <tlc::syntax::IsASTNode T>
    using FnNodeT = void (*)(T const&);

    inline static const tlc::fs::path filepath =
        "toy-lang/test/unit/parse.toy";

protected:
    TLC_TEST_GENERATE_PARSE_OVERLOAD(Expr);
    TLC_TEST_GENERATE_PARSE_OVERLOAD(Type);
    TLC_TEST_GENERATE_PARSE_OVERLOAD(Decl);
    TLC_TEST_GENERATE_PARSE_OVERLOAD(Stmt);

    template <tlc::syntax::IsASTNode T>
    static auto cast(tlc::syntax::Node const& node) -> T {
        REQUIRE(tlc::syntax::matchAstType<T>(node));
        return tlc::syntax::astCast<T>(node);
    }

    struct AssertExpr {
        struct Identifier {
            tlc::Opt<tlc::Str> path;
        };

        struct Integer {
            tlc::Opt<tlc::i64> value;
        };

        struct Float {
            tlc::Opt<tlc::f64> value;
        };

        struct Boolean {
            tlc::Opt<tlc::b8> value;
        };

        struct Tuple {
            tlc::Opt<tlc::szt> size;
            tlc::Opt<FnNodes> assert_children;
        };

        struct Array {
            tlc::Opt<tlc::szt> size;
            tlc::Opt<FnNodes> assert_children;
        };

        struct Access {
            tlc::Opt<FnNode> assert_object;
            tlc::Opt<FnNode> assert_field;
        };

        struct FnApp {
            tlc::Opt<FnNode> assert_callee;
            tlc::Opt<FnNode> assert_args;
        };

        struct Subscript {
            tlc::Opt<FnNode> assert_collection;
            tlc::Opt<FnNode> assert_subscript;
        };

        struct Prefix {
            tlc::Opt<tlc::token::EToken> op;
            tlc::Opt<FnNode> assert_operand;
        };

        TLC_TEST_GENERATE_ASSERT_DECL(identifier, Identifier);
        TLC_TEST_GENERATE_ASSERT_DECL(integer, Integer);
        TLC_TEST_GENERATE_ASSERT_DECL(fl0at, Float);
        TLC_TEST_GENERATE_ASSERT_DECL(boolean, Boolean);
        TLC_TEST_GENERATE_ASSERT_DECL(tuple, Tuple);
        TLC_TEST_GENERATE_ASSERT_DECL(array, Array);
        TLC_TEST_GENERATE_ASSERT_DECL(access, Access);
        TLC_TEST_GENERATE_ASSERT_DECL(fnApp, FnApp);
        TLC_TEST_GENERATE_ASSERT_DECL(subscript, Subscript);
        TLC_TEST_GENERATE_ASSERT_DECL(prefix, Prefix);
    };

    struct AssertType {
        struct Identifier {
            tlc::Opt<tlc::b8> fundamental;
            tlc::Opt<tlc::b8> imported;
            tlc::Opt<tlc::Str> path;
        };

        struct Tuple {
            tlc::Opt<tlc::szt> size;
            tlc::Opt<FnNodes> assert_children;
        };

        struct Infer {
            tlc::Opt<FnNode> assert_expr;
        };

        struct Array {
            tlc::Opt<tlc::szt> dim;
            tlc::Opt<FnNode> assert_type;
            tlc::Opt<FnNodeT<tlc::syntax::type::Array>> assert_self;
        };

        struct Function {
            tlc::Opt<FnNode> assert_args;
            tlc::Opt<FnNode> assert_result;
        };

        TLC_TEST_GENERATE_ASSERT_DECL(identifier, Identifier);
        TLC_TEST_GENERATE_ASSERT_DECL(tuple, Tuple);
        TLC_TEST_GENERATE_ASSERT_DECL(infer, Infer);
        TLC_TEST_GENERATE_ASSERT_DECL(array, Array);
        TLC_TEST_GENERATE_ASSERT_DECL(function, Function);
    };

    struct AssertDecl {
        struct Identifier {
            tlc::Opt<tlc::b8> constant;
            tlc::Opt<FnNode> assert_identifier;
            tlc::Opt<FnNode> assert_type;
        };

        struct Tuple {};

        TLC_TEST_GENERATE_ASSERT_DECL(identifier, Identifier);
        // TLC_TEST_GENERATE_ASSERT_DECL(tuple, Tuple);
    };
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParseTestFixture, __VA_ARGS__)

#endif // TLC_TEST_UNIT_PARSE_TEST_HPP
