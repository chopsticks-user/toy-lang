#include "parse_error.hpp"
#include "core/core.hpp"

namespace tlc {
    template <>
    auto Error<parse::EContext, parse::EReason>::
    message() const -> Str {
        using parse::EContext;
        using parse::EReason;
        return "";
    }
}
