#ifndef TLC_PARSE_COMBINATOR_HPP
#define TLC_PARSE_COMBINATOR_HPP

#include "core/core.hpp"
#include "context.hpp"
#include "panic.hpp"
#include "stream.hpp"

namespace tlc::parse {
    struct ParserCombinatorResult {
        bool success = false;
        Vec<syntax::Node> nodes;
    };

    using ParserCombinator = std::function<
        ParserCombinatorResult(
            Context& context, Stream& stream, Panic& panic
        )
    >;

#define TLC_PARSER_COMBINATOR_PROTOTYPE \
    [=]([[maybe_unused]] Context& context, [[maybe_unused]] Stream& stream, [[maybe_unused]] Panic& panic) \
        -> ParserCombinatorResult

    auto match(std::same_as<token::EToken> auto... types) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            auto const v = Vec{types...};
            if (!stream.match(types...)) {
                return {};
            }
            return {
                true,
                {syntax::TokenWrapper{stream.current()}}
            };
        };
    }

    auto zeroOrMany(std::same_as<token::EToken> auto... types) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            ParserCombinatorResult result{true, {}};
            while (stream.match(types...)) {
                result.nodes.emplace_back(syntax::TokenWrapper{stream.current()});
            }
            return result;
        };
    }

    auto oneOrMany(std::same_as<token::EToken> auto... types) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            // return (match(type) & (*type))(context, stream, panic);
            auto result = (*(types, ...))(context, stream, panic);
            if (result.nodes.empty()) {
                result.success = false;
            }
            return result;
        };
    }

    auto pcOr(
        std::same_as<ParserCombinator> auto const&... pc
    ) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            auto v = Vec<std::reference_wrapper<const ParserCombinator>>{pc...};

            if (ParserCombinatorResult result{};
                rng::any_of(std::execution::seq, [&](auto r) {
                    result = r(context, stream, panic);
                    return result.success;
                })) {
                return result;
            }

            return {};
        };
    }

    auto pcAnd(
        std::same_as<ParserCombinator> auto const&... pc
    ) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            auto v = Vec<std::reference_wrapper<const ParserCombinator>>{pc...};

            if (ParserCombinatorResult result{};
                rng::all_of(std::execution::seq, [&](auto r) {
                    auto&& [success, nodes] = r(context, stream, panic);
                    if (!success) {
                        // todo: revert to the original state
                        result = {};
                        return false;
                    }

                    result.nodes.append_range(nodes);
                    return true;
                })) {
                result.success = true;
                return result;
            }

            return {};
        };
    }
}

#endif // TLC_PARSE_COMBINATOR_HPP
