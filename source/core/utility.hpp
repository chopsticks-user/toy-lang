#ifndef TLC_CORE_FILE_HPP
#define TLC_CORE_FILE_HPP

#include "type.hpp"
#include "config.hpp"

#include <fstream>
#include <filesystem>
#include <source_location>

namespace tlc {
    namespace fs = std::filesystem;

    class FileReader final {
    public:
        FileReader() = default;

        explicit FileReader(fs::path const& filepath) {
            auto m_ifs = std::make_unique<std::ifstream>();
            m_ifs->open(filepath);

            // todo: specific exception
            if (!m_ifs->is_open()) {
                throw std::runtime_error("Failed to open " + filepath.string());
            }

            m_is = std::move(m_ifs);
        }

        explicit FileReader(std::istringstream iss)
            : m_is(std::make_unique<std::istringstream>(std::move(iss))) {}

        auto skipLine() const -> void {
            std::string dummy;
            m_is->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        [[nodiscard]] auto advance() const -> c8 {
            return static_cast<c8>(m_is->get());
        }

        auto revert() const -> void {
            m_is->unget();
        }

        [[nodiscard]] auto peek() const -> c8 {
            return static_cast<c8>(m_is->peek());
        }

        [[nodiscard]] auto eof() const -> bool {
            return m_is->eof();
        }

    private:
        Ptr<std::istream> m_is;
    };

    struct ThousandsSep final : std::numpunct<char> {
        auto do_thousands_sep() const -> char override { return ','; }
        auto do_grouping() const -> Str override { return "\3"; }
    };

    auto intStringWithThousandsSep(i64 value) -> Str;

    enum class ELogLevel {
        Info, Warning, Fatal
    };

    auto log(
        ELogLevel level, StrV message,
        std::source_location sl = std::source_location::current()
    ) -> void;

    auto exitOnInternalError(
        StrV message, std::source_location sl = std::source_location::current()
    ) -> void;

    class ScopeTimer {
    public:
        explicit ScopeTimer(Fn<void(i64)>&& onDestroyed);
        ~ScopeTimer();

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
        Fn<void(i64)> m_onDestroyed;
    };

    class ScopeReporter {
    public:
        explicit ScopeReporter(
            std::source_location sl = std::source_location::current()
        );
        ~ScopeReporter() noexcept;

    private:
        ScopeTimer m_timer;
    };

#ifdef TLC_CONFIG_BUILD_DEBUG
#define TLC_LOG(level, message) \
    log(level, message)
#define TLC_SCOPE_REPORTER() \
    tlc::ScopeReporter tlc_scope_reporter_##__LINE__{}
#else
#define TLC_LOG(level, message)
#define TLC_SCOPE_REPORTER()
#endif
};

#endif // TLC_CORE_FILE_HPP
