#include "combinators.hpp"

namespace tlc::parse {
    auto match(std::same_as<token::EToken> auto... types) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            if (!stream.match(types...)) {
                return {false, {}};
            }
            return {
                true,
                {syntax::TokenWrapper{stream.current()}}
            };
        };
    }

    auto operator~(token::EToken const type) -> ParserCombinator {
        return match(type);
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

    auto operator*(token::EToken const type) -> ParserCombinator {
        return zeroOrMany(type);
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

    auto operator+(token::EToken const type) -> ParserCombinator {
        return oneOrMany(type);
    }

    auto operator|(
        ParserCombinator const& pc1, ParserCombinator const& pc2
    ) -> ParserCombinator {
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            if (auto result = pc1(context, stream, panic);
                result.success) {
                return result;
            }
            return pc2(context, stream, panic);
        };
    }

    ParserCombinator operator&(
        ParserCombinator const& pc1, ParserCombinator const& pc2
    ) {
        ParserCombinator pc3(pc1);
        return TLC_PARSER_COMBINATOR_PROTOTYPE {
            if (auto result1 = pc1(context, stream, panic);
                result1.success) {
                if (auto&& [success2, nodes2] = pc2(context, stream, panic);
                    success2) {
                    result1.nodes.append_range(nodes2);
                    return result1;
                }
            }

            // todo: revert states if pc2 fails
            return {false, {}};
        };
    }
}
