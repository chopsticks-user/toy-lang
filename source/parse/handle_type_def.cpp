#include "parse.hpp"

namespace tlc::parse {
    auto Parse::handleTypeDef([[maybe_unused]] token::Token const& visibility) -> ParseResult {
        return {};
    }
}
