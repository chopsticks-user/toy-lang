#include "parse_error.hpp"

namespace tlc {
    template <>
    auto Error<parse::EParseErrorContext, parse::EParseErrorReason>::
    message() const -> Str {
        using parse::EParseErrorContext;
        using parse::EParseErrorReason;
        return "";
    }
}
