#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleTraitDef([[maybe_unused]] token::Token const& visibility) -> ParseResult {
        return {};
    }
}
