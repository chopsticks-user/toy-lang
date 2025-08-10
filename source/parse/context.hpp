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
        using enum EParseErrorContext;

        constexpr Context(fs::path const& filepath,
                          TokenStream& tokenStream,
                          LocationTracker& locationTracker,
                          TErrorCollector& errorCollector,
                          EParseErrorContext const errorContext)
            : m_filepath{filepath},
              m_tokenStream{tokenStream},
              m_locationTracker{locationTracker},
              m_errorCollector{errorCollector},
              m_errorContext{errorContext},
              m_location{tokenStream.peek().location()} {
            m_tokenStream.markBacktrack();
        }

        constexpr ~Context() {
            if (!m_backtracked) {
                m_tokenStream.removeBacktrack();
                m_errorCollector(std::move(m_errors));
            }
            // m_locationTracker.pop();
        }

        constexpr auto backtrack() -> void {
            if (m_backtracked) {
                return;
            }
            m_backtracked = true;
            m_tokenStream.backtrack();
            m_errors.clear();
        }

        constexpr auto backtrackIf(b8 const cond) -> b8 {
            if (cond) {
                backtrack();
                return true;
            }
            return false;
        }

        constexpr auto emit(EParseErrorReason const reason) -> void {
            m_errors.emplace_back(TError::Params{
                .filepath = m_filepath,
                .location = m_locationTracker.current(),
                .context = m_errorContext,
                .reason = reason,
            });
        }

        constexpr auto emitIf(
            b8 const cond, EParseErrorReason const reason
        ) -> b8 {
            if (cond) {
                emit(reason);
                return true;
            }
            return false;
        }

        constexpr auto emitIfNodeEmpty(
            syntax::Node const& node,
            EParseErrorReason const reason
        ) -> b8 {
            if (syntax::isEmptyNode(node) ||
                syntax::matchAstType<syntax::RequiredButMissing>(node)) {
                emit(reason);
                return true;
            }
            return false;
        }

        constexpr auto emitIfLexemeNotPresent(
            lexeme::Lexeme const& lexeme_,
            EParseErrorReason const reason
        ) -> b8 {
            if (!m_tokenStream.match(lexeme_)) {
                emit(reason);
                return true;
            }
            return false;
        }

        constexpr auto location() const -> Location {
            return m_location;
        }

        constexpr auto stream() const -> TokenStream& {
            return m_tokenStream;
        }

        constexpr auto to(EParseErrorContext const context) -> void {
            m_errorContext = context;
        }

        constexpr auto filepath() const noexcept -> fs::path const& {
            return m_filepath;
        }

    private:
        fs::path const& m_filepath;
        TokenStream& m_tokenStream;
        LocationTracker& m_locationTracker;
        TErrorCollector& m_errorCollector;
        EParseErrorContext m_errorContext;

        b8 m_backtracked{false};
        Vec<TError> m_errors{};
        Location const m_location;
    };
}

#endif // TLC_PARSE_CONTEXT_HPP
