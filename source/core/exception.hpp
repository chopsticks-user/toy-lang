#ifndef TLC_CORE_EXCEPTION_HPP
#define TLC_CORE_EXCEPTION_HPP

#include "type.hpp"
#include "singleton.hpp"
#include "file.hpp"

namespace tlc {
    class Exception : public std::runtime_error {
    public:
        explicit Exception(Str message): std::runtime_error(std::move(message)) {}

        explicit Exception(fs::path filepath, Str message)
            : std::runtime_error("[" + filepath.string() + " @0:0]" + std::move(message)),
              m_filepath{std::move(filepath)} {}

        Exception(
            fs::path filepath, u64 const line, u64 const column,
            Str message
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

    struct InternalError final : Exception {
        explicit InternalError(Str message) : Exception(std::move(message)) {}

        InternalError(fs::path filepath, Str message)
            : Exception(std::move(filepath), std::move(message)) {}

        InternalError(
            fs::path filepath, u64 const line, u64 const column,
            Str message
        ): Exception{
            std::move(filepath), line, column, "Internal error: " + std::move(message)
        } {}
    };

    // todo: code should be a reference
    struct CompileError final : Exception {
        CompileError(
            fs::path filepath, u64 const line, u64 const column,
            Str message, Str code
        ): Exception{
            std::move(filepath), line, column, std::move(message) + "\n"
            + std::move(code)
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
