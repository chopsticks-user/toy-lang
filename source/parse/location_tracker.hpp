#ifndef TLC_PARSE_LOCATION_TRACKER_HPP
#define TLC_PARSE_LOCATION_TRACKER_HPP

#include "core/core.hpp"
#include "token_stream.hpp"

namespace tlc::parse {
    class LocationTracker final {
    public:
        explicit LocationTracker(TokenStream const& stream)
            : m_stream{stream} {}

    private:
        TokenStream m_stream;
    };
}


#endif // TLC_PARSE_LOCATION_TRACKER_HPP
