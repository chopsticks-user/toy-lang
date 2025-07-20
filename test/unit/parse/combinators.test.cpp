#include <catch2/catch_test_macros.hpp>

#include "parse/combinator.hpp"

class ParserCombinatorTestFixture {
protected:
    auto initialize(tlc::Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_stream.emplace(tlc::lex::Lexer::operator()(std::move(iss)));
    }

    auto invoke(tlc::parse::ParserCombinator const& pc)
        -> tlc::parse::ParserCombinatorResult {
        // todo: no value error
        return pc(m_context, m_stream.value(), m_panic);
    }

    // static auto tokenType(tlc::syntax::Node const& node) -> tlc::token::EToken {
    //     return tlc::syntax::astCast<tlc::syntax::TokenWrapper>(node)
    //            .token().type();
    // }

private:
    tlc::parse::Context m_context{};
    tlc::parse::Panic m_panic{};
    tlc::Opt<tlc::parse::TokenStream> m_stream;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParserCombinatorTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("ParserCombinators: Match", "[Parse]") {
    using enum tlc::token::EToken;

    initialize("x=5;");

    {
        auto [success, nodes] = invoke(
            tlc::parse::match(Integer10Literal, Identifier, Semicolon)
        );
        REQUIRE(success);
        REQUIRE(nodes.size() == 1);
        REQUIRE(nodes[0].type() == tlc::token::EToken::Identifier);
    }
    {
        auto [success, nodes] = invoke(
            tlc::parse::match(Equal)
        );
        REQUIRE(success);
        REQUIRE(nodes.size() == 1);
        REQUIRE(nodes[0].type() == tlc::token::EToken::Equal);
    }
    {
        auto [success, nodes] = invoke(
            tlc::parse::match(Integer10Literal, Identifier, Semicolon)
        );
        REQUIRE(success);
        REQUIRE(nodes.size() == 1);
        REQUIRE(nodes[0].type() == tlc::token::EToken::Integer10Literal);
    }
    {
        auto [success, nodes] = invoke(
            tlc::parse::match(Integer10Literal, Semicolon)
        );
        REQUIRE(success);
        REQUIRE(nodes.size() == 1);
        REQUIRE(nodes[0].type() == tlc::token::EToken::Semicolon);
    }
    {
        auto [success, nodes] = invoke(
            tlc::parse::match(Integer10Literal, Semicolon)
        );
        REQUIRE_FALSE(success);
        REQUIRE(nodes.empty());
    }
}
