export module parse:tracker;

import :stream;
import core;

namespace tlc::parse {
    export class LocationTracker final {
    public:
        explicit constexpr LocationTracker(TokenStream const& stream)
            : m_stream{stream} {}

        constexpr auto push() -> Location {
            auto const next = m_stream.peek().location();
            m_locations.push(m_stream.peek().location());
            return next;
        }

        constexpr auto top() -> Location {
            if (m_locations.empty()) {
                exitOnInternalError("m_locations.empty()");
            }
            return m_locations.top();
        }

        constexpr auto current() const -> Location {
            return m_stream.current().location();
        }

        constexpr auto pop() -> Location {
            auto const top = m_locations.top();
            m_locations.pop();
            return top;
        }

    private:
        TokenStream const& m_stream;
        Stack<Location> m_locations{};
    };
}
