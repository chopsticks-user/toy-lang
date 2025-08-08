#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleEnumDef([[maybe_unused]] token::Token const& visibility) -> ParseResult {
        return {};
    }
}
