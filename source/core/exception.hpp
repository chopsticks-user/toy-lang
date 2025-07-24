#ifndef TLC_CORE_EXCEPTION_HPP
#define TLC_CORE_EXCEPTION_HPP

#include "type.hpp"
#include "singleton.hpp"

namespace tlc {
    class Exception : public std::runtime_error {
    public:
        explicit Exception(Str const& message): std::runtime_error(message) {}

        explicit Exception(Str const& filepath, Str const& message)
            : std::runtime_error("[" + filepath + " @0:0]" + message) {}

        Exception(
            Str const& filepath, const u64 line, const u64 column,
            Str const& message
        ): std::runtime_error(
               "[" + filepath + " @" + std::to_string(line) + ":" +
               std::to_string(column) + "] " + message
           ), m_filepath(filepath), m_line(line), m_column(column) {}

        [[nodiscard]] auto filepath() const -> StrV {
            return m_filepath;
        }

        [[nodiscard]] auto line() const -> szt {
            return m_line;
        }

        [[nodiscard]] auto column() const -> szt {
            return m_column;
        }

    protected:
        Str m_filepath;
        u64 m_line = 0;
        u64 m_column = 0;
    };

    struct InternalError final : Exception {
        explicit InternalError(Str const& message) : Exception(message) {}

        InternalError(Str const& filepath, Str const& message)
            : Exception(filepath, message) {}

        InternalError(
            Str const& filepath, const u64 line, const u64 column,
            Str const& message
        ): Exception{filepath, line, column, "Internal error: " + message} {}
    };

    struct CompileError final : Exception {
        CompileError(
            Str const& filepath, const u64 line, const u64 column,
            Str const& message, Str const& code
        ): Exception{
            filepath, line, column, message + "\n" + code
        } {}
    };

    // todo: use deducing this
    // How C++23 Changes the Way We Write Code - Timur Doumler - CppCon 2022
    class GlobalExceptionCollector : public Singleton<GlobalExceptionCollector> {
    public:
        [[nodiscard]] auto size() const -> szt {
            return m_collected.size();
        }

        [[nodiscard]] auto empty() const -> szt {
            return m_collected.empty();
        }

        // todo: handle out of memory, !InternalException
        auto add(Exception e) -> void {
            m_collected.emplace_back(std::make_unique<Exception>(std::move(e)));
        }

        auto throwAllIfExists() -> void {
            if (empty()) {
                return;
            }

            Str combinedMesg;

            // todo: handle out of memory
            for (auto& e : m_collected) {
                combinedMesg += e->what() + "\n"s;
                e.reset();
            }
            m_collected.clear();

            throw Exception(combinedMesg);
        }

    protected:
        GlobalExceptionCollector() = default;

    private:
        Vec<Ptr<Exception>> m_collected;
    };
}

#endif // TLC_CORE_EXCEPTION_HPP
