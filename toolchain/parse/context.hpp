// ReSharper disable CppDFAUnreachableFunctionCall
#ifndef TLC_PARSE_CONTEXT_HPP
#define TLC_PARSE_CONTEXT_HPP

#include "core/core.hpp"
#include "syntax/syntax.hpp"

#include "parse_error.hpp"
#include "token_stream.hpp"
#include "location_tracker.hpp"

namespace tlc::parse {
    using SynchronizingSet = HashSet<lexeme::Lexeme>;

    class Context final {
        using TError = Error<EContext, EReason>;
        using TErrorCollector = ErrorCollector<
            EContext, EReason
        >;

    public:
        struct Params final {
            fs::path const& filepath;
            TokenStream& tokenStream;
            LocationTracker& locationTracker;
            TErrorCollector& errorCollector;
            EContext const errorContext;
            b8 const isSubroutine = false;
            Context* parent = nullptr;
            Opt<syntax::OpPrecedence> minPrecedence = {};
            Opt<token::Token> visibility = {};
        };

        static constexpr auto global(
            fs::path const& filepath, TokenStream& tokenStream,
            LocationTracker& locationTracker, TErrorCollector& errorCollector,
            EContext const errorContext,
            b8 const isSubroutine
        ) noexcept -> Context {
            return Params{
                .filepath = filepath,
                .tokenStream = tokenStream,
                .locationTracker = locationTracker,
                .errorCollector = errorCollector,
                .errorContext = errorContext,
                .isSubroutine = isSubroutine,
            };
        }

        static constexpr auto enter(
            EContext const eContext,
            Context& parent,
            Opt<syntax::OpPrecedence> minPrecedence = {},
            Opt<token::Token> visibility = {}
        ) noexcept -> Context {
            return Params{
                .filepath = parent.filepath(),
                .tokenStream = parent.stream(),
                .locationTracker = parent.tracker(),
                .errorCollector = parent.collector(),
                .errorContext = eContext,
                .isSubroutine = parent.isSubroutine(),
                .parent = &parent,
                .minPrecedence = std::move(minPrecedence),
                .visibility = std::move(visibility),
            };
        }

    public:
        // ReSharper disable once CppNonExplicitConvertingConstructor
        constexpr Context(Params&& params) noexcept
            : m_filepath{params.filepath},
              m_tokenStream{params.tokenStream},
              m_locationTracker{params.locationTracker},
              m_errorCollector{params.errorCollector},
              m_errorContext{params.errorContext},
              m_isSubroutine{params.isSubroutine},
              m_parent{std::move(params.parent)},
              m_minPrecedence{params.minPrecedence.value_or(0)},
              m_visibility{
                  params.visibility.value_or(
                      token::Token{
                          lexeme::empty, "",
                          params.tokenStream.peek().location()
                      })
              },
              m_location{params.tokenStream.peek().location()} {
            m_tokenStream.markBacktrack();
        }

        // todo: noexcept
        constexpr ~Context() {
            if (!m_backtracked) {
                m_tokenStream.removeBacktrack();
                if (m_parent) {
                    m_parent->appendErrors(std::move(m_errors));
                }
                else {
                    m_errorCollector(std::move(m_errors));
                }
            }
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

        constexpr auto emit(EReason const reason) -> void {
            m_errors.emplace_back(TError::Params{
                .filepath = m_filepath,
                .location = m_locationTracker.current(),
                .context = m_errorContext,
                .reason = reason,
            });
        }

        constexpr auto emitIf(
            b8 const cond, EReason const reason
        ) -> b8 {
            if (cond) {
                emit(reason);
                return true;
            }
            return false;
        }

        constexpr auto emitIfNodeMissing(
            syntax::Node const& node,
            EReason const reason
        ) -> b8 {
            return emitIf(syntax::match<syntax::RequiredButMissing>(node),
                          reason);
        }

        constexpr auto emitIfNodeEmpty(
            syntax::Node const& node,
            EReason const reason
        ) -> b8 {
            return emitIf(syntax::empty(node), reason);
        }

        constexpr auto emitIfLexemeNotPresent(
            lexeme::Lexeme const& lexeme_,
            EReason const reason
        ) -> b8 {
            return emitIf(!m_tokenStream.match(lexeme_), reason);
        }

        [[nodiscard]] constexpr auto location() const noexcept -> Location {
            return m_location;
        }

        [[nodiscard]] constexpr auto stream() const noexcept -> TokenStream& {
            return m_tokenStream;
        }

        constexpr auto to(EContext const context) noexcept -> void {
            m_errorContext = context;
        }

        [[nodiscard]] constexpr auto filepath() const noexcept -> fs::path const& {
            return m_filepath;
        }

        constexpr auto appendErrors(Vec<TError> errors) -> void {
            m_errors.append_range(std::move(errors));
        }

        constexpr auto minPrecedence() const noexcept -> syntax::OpPrecedence {
            return m_minPrecedence;
        }

        constexpr auto visibility() const noexcept -> token::Token const& {
            return m_visibility;
        }

        constexpr auto tracker() const noexcept -> LocationTracker& {
            return m_locationTracker;
        }

        constexpr auto collector() const noexcept -> TErrorCollector& {
            return m_errorCollector;
        }

        constexpr auto isSubroutine() const noexcept -> b8 {
            return m_isSubroutine;
        }

        constexpr auto synchronize(
            SynchronizingSet const& synchronizingSet
        ) const -> void {
            if (synchronizingSet.empty()) {
                return;
            }
            while (!(m_tokenStream.done() ||
                synchronizingSet.contains(m_tokenStream.peek().lexeme()))) {
                // todo: save dropped tokens
                m_tokenStream.advance();
            }
        }

        constexpr auto hasParent() const noexcept -> b8 {
            return m_parent != nullptr;
        }

    private:
        fs::path const& m_filepath;
        TokenStream& m_tokenStream;
        LocationTracker& m_locationTracker;
        TErrorCollector& m_errorCollector;
        EContext m_errorContext;
        b8 const m_isSubroutine;
        Context* m_parent;
        syntax::OpPrecedence m_minPrecedence;
        token::Token m_visibility;

        b8 m_backtracked{false};
        Vec<TError> m_errors{};
        Location const m_location;
    };
}

#endif // TLC_PARSE_CONTEXT_HPP
