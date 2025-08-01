#ifndef TLC_PARSE_LOCATION_TRACKER_HPP
#define TLC_PARSE_LOCATION_TRACKER_HPP

#include "core/core.hpp"
#include "token_stream.hpp"

namespace tlc::parse {
    class ScopedLocation;

    class LocationTracker final {
    public:
        explicit LocationTracker(TokenStream const& stream)
            : m_stream{stream} {}

        auto scopedLocation() noexcept -> ScopedLocation;

        auto push() -> Location {
            auto const next = m_stream.peek().location();
            m_locations.push(m_stream.peek().location());
            return next;
        }

        auto top() -> Location {
            if (m_locations.empty()) {
                exitOnInternalError("m_locations.empty()");
            }
            return m_locations.top();
        }

        auto current() const -> Location {
            return m_stream.current().location();
        }

        auto pop() -> Location {
            auto const top = m_locations.top();
            m_locations.pop();
            return top;
        }

    private:
        TokenStream const& m_stream;
        Stack<Location> m_locations{};
    };

    class ScopedLocation {
    public:
        explicit ScopedLocation(LocationTracker& tracker) noexcept
            : m_tracker{tracker}, m_location{tracker.push()} {}

        ~ScopedLocation() noexcept {
            m_tracker.pop();
        }

        ScopedLocation(ScopedLocation&&) = delete;
        ScopedLocation& operator=(ScopedLocation&&) = delete;
        ScopedLocation(ScopedLocation const&) = delete;
        ScopedLocation& operator=(ScopedLocation const&) = delete;

        auto operator*() const noexcept -> Location {
            return m_location;
        }

    private:
        LocationTracker& m_tracker;
        Location m_location;
    };

    inline auto LocationTracker::scopedLocation() noexcept -> ScopedLocation {
        return ScopedLocation{*this};
    }
}


#endif // TLC_PARSE_LOCATION_TRACKER_HPP
