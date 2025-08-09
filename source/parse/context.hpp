#ifndef TLC_PARSE_CONTEXT_HPP
#define TLC_PARSE_CONTEXT_HPP

#include "core/core.hpp"

#include "parse_error.hpp"
#include "token_stream.hpp"
#include "location_tracker.hpp"

namespace tlc::parse {
    class Context final {
        using TError = Error<EParseErrorContext, EParseErrorReason>;
        using TErrorCollector = ErrorCollector<EParseErrorContext, EParseErrorReason>;

    public:
        constexpr Context(fs::path const& filepath,
                          TokenStream& tokenStream,
                          LocationTracker& locationTracker,
                          TErrorCollector& errorCollector,
                          EParseErrorContext const errorContext)
            : m_filepath{filepath},
              m_tokenStream{tokenStream},
              m_locationTracker{locationTracker},
              m_errorCollector{errorCollector},
              m_errorContext{errorContext} {
            m_tokenStream.markBacktrack();
            m_locationTracker.push();
        }

        constexpr ~Context() {
            if (!m_backtracked) {
                m_tokenStream.removeBacktrack();
                m_errorCollector(std::move(m_errors));
            }
            m_locationTracker.pop();
        }

        constexpr auto backtrack() -> void {
            if (m_backtracked) {
                return;
            }
            m_backtracked = true;
            m_tokenStream.backtrack();
            m_errors.clear();
        }

        constexpr auto emit(EParseErrorReason const reason) -> void {
            m_errors.emplace_back(TError::Params{
                .filepath = m_filepath,
                .location = m_locationTracker.current(),
                .context = m_errorContext,
                .reason = reason,
            });
        }

        constexpr auto location() const -> Location {
            return m_locationTracker.top();
        }

    private:
        fs::path const& m_filepath;
        TokenStream& m_tokenStream;
        LocationTracker& m_locationTracker;
        TErrorCollector& m_errorCollector;
        EParseErrorContext const m_errorContext;

        b8 m_backtracked{false};
        Vec<TError> m_errors{};
    };
}

#endif // TLC_PARSE_CONTEXT_HPP
