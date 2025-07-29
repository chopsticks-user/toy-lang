#ifndef TLC_PARSE_PANIC_HPP
#define TLC_PARSE_PANIC_HPP

#include "core/core.hpp"
#include "syntax/syntax.hpp"
#include "lex/lex.hpp"

namespace tlc::parse {
    class Error final {
    public:
        enum class Reason {
            NotAnError,

            /**
             *
             */
            MissingSymbol,

            /**
             *
             */
            MissingEnclosingSymbol,

            /**
             *
             */
            MissingExpr,

            /**
             *
             */
            MissingType,

            /**
             *
             */
            MissingId,

            /**
             *
             */
            MissingDecl,

            /**
             *
             */
            MissingStmt,

            /**
             *
             */
            Unknown,
        };

        enum class Context {
            Unknown,
            Tuple, Array, Access, TypeInfer, Record, IdDecl, TupleDecl,
            Stmt, LetStmt, BlockStmt, PrefaceStmt, DeferStmt, AssignStmt,
            ExprStmt, CondStmt,
        };

        struct Params {
            fs::path filepath;
            token::Token::Coords location;
            Context context = Context::Unknown;
            Reason reason = Reason::NotAnError;
            Str info;
        };

    public:
        Error() = default;

        explicit Error(Params params)
            : m_filepath{std::move(params.filepath)},
              m_location{std::move(params.location)},
              m_context{params.context},
              m_reason{params.reason},
              m_info{std::move(params.info)} {}

        explicit operator CompileError() const {
            return {
                m_filepath, m_location.first, m_location.second,
                message(), ""
            };
        }

        [[nodiscard]] auto message() const -> Str;

        [[nodiscard]] auto reason() const -> Reason {
            return m_reason;
        }

    private:
        [[nodiscard]] auto endOfSentenceWithInfo() const -> Str {
            return m_info.empty() ? " " + m_info + "." : ".";
        }

    private:
        fs::path m_filepath;
        token::Token::Coords m_location;
        Context m_context = Context::Unknown;
        Reason m_reason = Reason::NotAnError;
        Str m_info;
    };

    class Panic final {
    public:
        explicit Panic(fs::path filepath)
            : m_filepath{std::move(filepath)} {}

        auto collect(Error error) -> void {
            if (error.reason() != Error::Reason::NotAnError) {
                m_errors.push_back(std::move(error));
            }
        }

        auto collect(Error::Params params) -> void {
            if (params.reason != Error::Reason::NotAnError) {
                params.filepath = m_filepath;
                m_errors.emplace_back(std::move(params));
            }
        }

    private:
        fs::path m_filepath;
        Vec<Error> m_errors;
    };
}

#endif // TLC_PARSE_PANIC_HPP
