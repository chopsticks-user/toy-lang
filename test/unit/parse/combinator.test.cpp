#include <catch2/catch_test_macros.hpp>

#include "parse/combinator.hpp"
#include "parse/parse.hpp"

class ParserCombinatorTestFixture {
    inline static const tlc::fs::path filepath =
        "toy-lang/test/unit/parse/combinator.toy";

protected:
    auto initialize(tlc::Str source) -> void {
        std::istringstream iss;
        iss.str(std::move(source));
        m_stream.emplace(tlc::lex::Lex::operator()(std::move(iss)));
        m_context = {};
        m_panic = tlc::parse::Panic{filepath};
    }

    auto invoke(tlc::parse::ParserCombinator const& pc)
        -> tlc::parse::ParserCombinatorResult {
        // todo: no value error
        return pc(m_context, m_stream.value(), m_panic);
    }

    auto stream() -> tlc::parse::TokenStream {
        REQUIRE(m_stream.has_value());
        return *m_stream;
    }

private:
    tlc::parse::Context m_context{};
    tlc::parse::Panic m_panic{filepath};
    tlc::Opt<tlc::parse::TokenStream> m_stream;
};

#define TEST_CASE_WITH_FIXTURE(...) \
    TEST_CASE_METHOD(ParserCombinatorTestFixture, __VA_ARGS__)

TEST_CASE_WITH_FIXTURE("ParserCombinator:", "[Parse][ParserCombinator]") {
    using enum tlc::token::EToken;
    //
    // initialize("-x-5");
    //
    // auto [success, nodes, _] =
    //     invoke(tlc::parse::handleExpr());
}

TEST_CASE_WITH_FIXTURE("ParserCombinator: Match", "[Parse][ParserCombinator]") {
    using enum tlc::token::EToken;

    SECTION("Sequential matches") {
        initialize("x=5;");

        {
            auto const result = invoke(
                tlc::parse::match(Integer10Literal, Identifier, Semicolon)
            );
            REQUIRE(result);
            REQUIRE(result.value().size() == 1);
            REQUIRE(result.value().front().type() == tlc::token::EToken::Identifier);
        }
        {
            auto const result = invoke(
                tlc::parse::match(Equal)
            );
            REQUIRE(result);
            REQUIRE(result.value().size() == 1);
            REQUIRE(result.value().front().type() == tlc::token::EToken::Equal);
        }
        {
            auto const result = invoke(
                tlc::parse::match(Integer10Literal, Identifier, Semicolon)
            );
            REQUIRE(result);
            REQUIRE(result.value().size() == 1);
            REQUIRE(result.value().front().type() == tlc::token::EToken::Integer10Literal);
        }
        {
            auto const result = invoke(
                tlc::parse::match(Integer10Literal, Semicolon)
            );
            REQUIRE(result);
            REQUIRE(result.value().size() == 1);
            REQUIRE(result.value().front().type() == tlc::token::EToken::Semicolon);
        }
        {
            auto const result = invoke(
                tlc::parse::match(Integer10Literal, Semicolon)
            );
            REQUIRE_FALSE(result);
        }
    }

    SECTION("Match with a predicate") {
        initialize("x");

        static auto const predicate =
            [](auto type) static { return type == Identifier; };

        auto result = invoke(
            tlc::parse::match(predicate)
        );

        REQUIRE(result);
        REQUIRE(result.value().size() == 1);
        REQUIRE(result.value().front().type() == tlc::token::EToken::Identifier);
        REQUIRE(result.value().front().str() == "x");

        initialize("525");

        result = invoke(
            tlc::parse::match(predicate)
        );

        REQUIRE_FALSE(result);
    }
}

TEST_CASE_WITH_FIXTURE("ParserCombinator: Many or 0", "[Parse][ParserCombinator]") {
    using enum tlc::token::EToken;

    SECTION("Consumes all tokens") {
        initialize("x y z t");

        auto const result = invoke(
            tlc::parse::many0(tlc::parse::match(Identifier))
        );
        REQUIRE(result);
        REQUIRE(result.value().size() == 4);

        REQUIRE(tlc::rng::all_of(
            *result, [](auto const& token) {
            return token.type() == Identifier;
            }
        ));

        REQUIRE(tlc::rng::equal(
            *result | tlc::rv::transform([](auto const& token) {
                return token.str();
                }),
            tlc::Vec{"x", "y", "z", "t"}
        ));

        // stream is at the last token
        REQUIRE(stream().current().str() == "t");
    }

    SECTION("Consumes nothing but is considered success") {
        initialize("x y z t");

        auto const result = invoke(
            tlc::parse::many0(tlc::parse::match(Integer10Literal))
        );
        REQUIRE(result);
        REQUIRE(result.value().size() == 0);

        // stream has not started
        REQUIRE(stream().peek().str() == "x");
    }

    SECTION("Consumes partially") {
        initialize("x y z 555 x");

        auto const result = invoke(
            tlc::parse::many0(tlc::parse::match(Identifier))
        );
        REQUIRE(result);
        REQUIRE(result.value().size() == 3);

        REQUIRE(tlc::rng::all_of(
            *result, [](auto const& token) {
            return token.type() == Identifier;
            }
        ));

        REQUIRE(tlc::rng::equal(
            *result | tlc::rv::transform([](auto const& token) {
                return token.str();
                }),
            tlc::Vec{"x", "y", "z"}
        ));

        // stream is at the last token
        REQUIRE(stream().current().str() == "z");
    }
}

TEST_CASE_WITH_FIXTURE("ParserCombinator: Many or 1", "[Parse][ParserCombinator]") {
    using enum tlc::token::EToken;

    SECTION("Consumes all tokens") {
        initialize("5 26 777 0");

        auto const result = invoke(
            tlc::parse::many1(tlc::parse::match(Integer10Literal))
        );
        REQUIRE(result);
        REQUIRE(result.value().size() == 4);

        REQUIRE(tlc::rng::all_of(
            *result, [](auto const& token) {
            return token.type() == Integer10Literal;
            }
        ));

        REQUIRE(tlc::rng::equal(
            *result | tlc::rv::transform([](auto const& token) {
                return token.str();
                }),
            tlc::Vec{"5", "26", "777", "0"}
        ));

        // stream is at the last token
        REQUIRE(stream().current().str() == "0");
    }

    SECTION("Consuming nothing is considered failure") {
        initialize("x y z t");

        auto const result = invoke(
            tlc::parse::many1(tlc::parse::match(Integer10Literal))
        );
        REQUIRE_FALSE(result);

        // stream has not started
        REQUIRE(stream().peek().str() == "x");
    }
}

TEST_CASE_WITH_FIXTURE("ParserCombinator: Any", "[Parse][ParserCombinator]") {
    using enum tlc::token::EToken;

    SECTION("Success") {
        initialize("525");

        auto const result = invoke(
            tlc::parse::any(
                tlc::parse::many1(tlc::parse::match(Identifier)),
                tlc::parse::match(Integer10Literal)
            )
        );

        REQUIRE(result);
        REQUIRE(result.value().size() == 1);
        REQUIRE(result.value().front().str() == "525");
        REQUIRE(result.value().front().type() == Integer10Literal);
        REQUIRE(stream().current().str() == "525");
    }

    SECTION("Failed") {
        initialize("525.255");

        auto const result = invoke(
            tlc::parse::any(
                tlc::parse::many1(tlc::parse::match(Identifier)),
                tlc::parse::match(Integer10Literal)
            )
        );

        REQUIRE_FALSE(result);
        REQUIRE(stream().current().str() == "");
    }
}

TEST_CASE_WITH_FIXTURE("ParserCombinator: Sequence", "[Parse][ParserCombinator]") {
    using enum tlc::token::EToken;

    SECTION("Success") {
        initialize("x = 525;");

        auto const result = invoke(
            tlc::parse::seq(
                tlc::parse::match(Identifier),
                tlc::parse::match(Equal),
                tlc::parse::match(Integer10Literal)
            )
        );

        REQUIRE(result);
        REQUIRE(result.value().size() == 3);

        REQUIRE(tlc::rng::equal(
            *result | tlc::rv::transform([](auto const& token) {
                return token.type();
                }),
            tlc::Vec{Identifier, Equal, Integer10Literal}
        ));

        REQUIRE(tlc::rng::equal(
            *result | tlc::rv::transform([](auto const& token) {
                return token.str();
                }),
            tlc::Vec{"x", "=", "525"}
        ));

        REQUIRE(stream().current().str() == "525");
    }

    SECTION("Failed") {
        initialize("x = 525;");

        auto const result = invoke(
            tlc::parse::seq(
                tlc::parse::match(Identifier),
                tlc::parse::match(Equal),
                tlc::parse::match(Identifier)
            )
        );

        REQUIRE_FALSE(result);
        REQUIRE(stream().current().str() == "");
    }
}

/**
 * Fixed on 7/24/25
 * Solution: Created struct BacktrackStates in parse::TokenStream to revert
 * all required states instead of just reverting "m_tokenIt"
 */
TEST_CASE_WITH_FIXTURE(
    "ParserCombinator: Combinator for identifier literals",
    "[Parse][ParserCombinator][Fixed]"
) {
    using enum tlc::token::EToken;
    using namespace tlc::parse;

    static auto parser = seq(
        many0(seq(match(Identifier), match(Colon2))),
        match(Identifier, FundamentalType, UserDefinedType)
    );

    SECTION("Imported identifier") {
        initialize("foo::bar");

        auto const result = invoke(parser);
        REQUIRE(result);
        REQUIRE(result.value().size() == 3);

        REQUIRE(tlc::rng::equal(
            *result | tlc::rv::transform([](auto const& token) {
                return token.str();
                }),
            tlc::Vec{"foo", "::", "bar"}
        ));
    }

    SECTION("Local identifiers") {
        initialize("foo");

        auto const result = invoke(parser);
        REQUIRE(result);
        REQUIRE(result.value().size() == 1);
        REQUIRE(result.value().front().str() == "foo");
        REQUIRE(result.value().front().type() == Identifier);
    }
}
