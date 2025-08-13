// ReSharper disable CppDFAUnreachableFunctionCall
export module parse:context;

import syntax;
import lexeme;
import :error;
import :tracker;
import :stream;

namespace tlc::parse {
    class Context final {
        using TError = Error<EParseErrorContext, EParseErrorReason>;
        using TErrorCollector = ErrorCollector<
            EParseErrorContext, EParseErrorReason
        >;

    public:
        using enum EParseErrorContext;

        struct Params final {
            fs::path const& filepath;
            TokenStream& tokenStream;
            LocationTracker& locationTracker;
            TErrorCollector& errorCollector;
            EParseErrorContext const errorContext;
            b8 const isSubroutine = false;
            Context* parent = nullptr;
            Opt<syntax::OpPrecedence> minPrecedence = {};
            Opt<token::Token> visibility = {};
        };

        static constexpr auto global(
            fs::path const& filepath, TokenStream& tokenStream,
            LocationTracker& locationTracker, TErrorCollector& errorCollector,
            EParseErrorContext const errorContext,
            b8 const isSubroutine
        ) -> Context {
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
            EParseErrorContext const eContext,
            Context& parent,
            Opt<syntax::OpPrecedence> minPrecedence = {},
            Opt<token::Token> visibility = {}
        ) -> Context {
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
        constexpr Context(Params&& params)
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

        constexpr auto emitIfNodeMissing(
            syntax::Node const& node,
            EParseErrorReason const reason
        ) -> b8 {
            return emitIf(syntax::match<syntax::RequiredButMissing>(node),
                          reason);
        }

        constexpr auto emitIfNodeEmpty(
            syntax::Node const& node,
            EParseErrorReason const reason
        ) -> b8 {
            return emitIf(syntax::empty(node), reason);
        }

        constexpr auto emitIfLexemeNotPresent(
            lexeme::Lexeme const& lexeme_,
            EParseErrorReason const reason
        ) -> b8 {
            return emitIf(!m_tokenStream.match(lexeme_), reason);
        }

        [[nodiscard]] constexpr auto location() const -> Location {
            return m_location;
        }

        [[nodiscard]] constexpr auto stream() const -> TokenStream& {
            return m_tokenStream;
        }

        constexpr auto to(EParseErrorContext const context) -> void {
            m_errorContext = context;
        }

        [[nodiscard]] constexpr auto filepath() const noexcept -> fs::path const& {
            return m_filepath;
        }

        constexpr auto appendErrors(Vec<TError> errors) -> void {
            m_errors.append_range(std::move(errors));
        }

        constexpr auto minPrecedence() const -> syntax::OpPrecedence {
            return m_minPrecedence;
        }

        constexpr auto visibility() const -> token::Token const& {
            return m_visibility;
        }

        constexpr auto tracker() const -> LocationTracker& {
            return m_locationTracker;
        }

        constexpr auto collector() const -> TErrorCollector& {
            return m_errorCollector;
        }

        constexpr auto isSubroutine() const -> b8 {
            return m_isSubroutine;
        }

        // constexpr auto parent() const -> Context& {
        //     return *m_parent;
        // }

        auto synchronize() -> void;

    private:
        fs::path const& m_filepath;
        TokenStream& m_tokenStream;
        LocationTracker& m_locationTracker;
        TErrorCollector& m_errorCollector;
        EParseErrorContext m_errorContext;
        b8 const m_isSubroutine;
        Context* m_parent;
        syntax::OpPrecedence m_minPrecedence;
        token::Token m_visibility;

        b8 m_backtracked{false};
        Vec<TError> m_errors{};
        Location const m_location;
    };
}
