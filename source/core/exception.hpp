#ifndef TLC_CORE_EXCEPTION_HPP
#define TLC_CORE_EXCEPTION_HPP

#include "type.hpp"
#include "singleton.hpp"
#include "utility.hpp"

namespace tlc {
    class Exception : public std::runtime_error {
    public:
        explicit Exception(Str const& message): std::runtime_error(message) {}

        explicit Exception(fs::path filepath, Str message)
            : std::runtime_error("[" + filepath.string() + " @0:0]" + std::move(message)),
              m_filepath{std::move(filepath)} {}

        Exception(
            fs::path filepath, u64 const line, u64 const column, Str message
        ): std::runtime_error(
               "[" + filepath.string() + " @" + std::to_string(line) + ":" +
               std::to_string(column) + "] " + std::move(message)
           ), m_filepath(std::move(filepath)), m_line(line), m_column(column) {}

        [[nodiscard]] auto filepath() const -> fs::path {
            return m_filepath;
        }

        [[nodiscard]] auto line() const -> szt {
            return m_line;
        }

        [[nodiscard]] auto column() const -> szt {
            return m_column;
        }

    protected:
        fs::path m_filepath;
        u64 m_line = 0;
        u64 m_column = 0;
    };

    struct InternalException final : Exception {
        explicit InternalException(Str const& message) : Exception(message) {}

        InternalException(fs::path filepath, Str message)
            : Exception(std::move(filepath), std::move(message)) {}

        InternalException(
            fs::path filepath, u64 const line, u64 const column,
            Str message
        ): Exception{
            std::move(filepath), line, column, "Internal error: " + std::move(message)
        } {}
    };

    // todo: code should be a reference
    struct CompileException final : Exception {
        CompileException(
            fs::path filepath, u64 const line, u64 const column,
            Str message, Str code
        ): Exception{
            std::move(filepath), line, column, std::move(message) + "\n"
            + std::move(code)
        } {}
    };

#define TLC_CORE_GENERATE_SINGLETON_BODY_PREFIX(className) \
    public: \
        static auto instance() -> className& { \
            return Singleton::instance<className>(); \
        } \
    protected: \
        className() = default;

    template <typename EContext, typename EReason>
    class Error final {
    public:
        struct Params final {
            fs::path filepath;
            Location location;
            EContext context;
            EReason reason;
            Str info;
        };

        Error() = default;
        explicit Error(Params params) : m_params{std::move(params)} {}

        explicit operator CompileException() const {
            return {
                m_params.filepath,
                m_params.location.line,
                m_params.location.column,
                message(), ""
            };
        }

        auto filepath(Opt<fs::path> fp = {}) -> fs::path {
            if (!fp) {
                m_params.filepath = std::move(*fp);
            }
            return m_params.filepath;
        }

        [[nodiscard]] auto context() const -> EContext {
            return m_params.context;
        }

        [[nodiscard]] auto reason() const -> EReason {
            return m_params.reason;
        }

        [[nodiscard]] auto message() const -> Str;

    private:
        Params m_params{};
    };

    template <typename EContext, typename EReason>
    class ErrorCollector : public Singleton {
        TLC_CORE_GENERATE_SINGLETON_BODY_PREFIX(ErrorCollector)

    public:
        [[nodiscard]] auto size() const -> szt {
            return m_collected.size();
        }

        [[nodiscard]] auto empty() const -> szt {
            return m_collected.empty();
        }

        auto collect(typename Error<EContext, EReason>::Params errorParams)
            -> ErrorCollector& {
            m_collected.emplace_back(errorParams);
            return *this;
        }

        auto collect(Error<EContext, EReason> error)
            -> ErrorCollector& {
            m_collected.push_back(std::move(error));
            return *this;
        }

    private:
        Vec<Error<EContext, EReason>> m_collected;
    };
}

#endif // TLC_CORE_EXCEPTION_HPP
