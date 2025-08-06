#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleFlagDef([[maybe_unused]] token::Token const& visibility) -> ParseResult {
        return {};
    }
}
