#ifndef TLC_CORE_FILE_HPP
#define TLC_CORE_FILE_HPP

#include "type.hpp"
#include "config.hpp"

namespace tlc {
    namespace fs = std::filesystem;

    class FileReader final {
    public:
        FileReader() = default;

        explicit constexpr FileReader(fs::path const& filepath) {
            auto m_ifs = std::make_unique<std::ifstream>();
            m_ifs->open(filepath);

            // todo: specific exception
            if (!m_ifs->is_open()) {
                throw std::runtime_error("Failed to open " + filepath.string());
            }

            m_is = std::move(m_ifs);
        }

        explicit constexpr FileReader(std::istringstream iss)
            : m_is(std::make_unique<std::istringstream>(std::move(iss))) {}

        constexpr auto skipLine() const -> void {
            std::string dummy;
            m_is->ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        [[nodiscard]] constexpr auto advance() const -> c8 {
            return static_cast<c8>(m_is->get());
        }

        constexpr auto revert() const -> void {
            m_is->unget();
        }

        [[nodiscard]] constexpr auto peek() const -> c8 {
            return static_cast<c8>(m_is->peek());
        }

        [[nodiscard]] constexpr auto eof() const -> bool {
            return m_is->eof();
        }

    private:
        Ptr<std::istream> m_is;
    };

    struct ThousandsSep final : std::numpunct<char> {
        auto do_thousands_sep() const -> char override { return ','; }
        auto do_grouping() const -> Str override { return "\3"; }
    };

    constexpr auto intStringWithThousandsSep(i64 value) -> Str {
        return std::format(
            std::locale{std::locale{""}, new ThousandsSep}, "{:L}", value
        );
    }

    enum class ELogLevel {
        Info, Warning, Fatal
    };

    constexpr auto log(
        ELogLevel const level, StrV message,
        std::source_location const sl = std::source_location::current()
    ) -> void {
        if constexpr (config::debugging) {
            switch (level) {
            case ELogLevel::Info: {
                std::println(
                    "\033[1;32m[INFO]\033[0m {}\n\t\t{}:{}",
                    message, sl.file_name(), sl.line()
                );
                return;
            }
            case ELogLevel::Warning: {
                std::println(
                    "\033[1;33m[WARN]\033[0m {}\n\t\t{}:{}",
                    message, sl.file_name(), sl.line()
                );
                return;
            }
            case ELogLevel::Fatal: {
                std::println(
                    "\033[1;31m[FATAL]\033[0m {}\n\t\t{}:{}",
                    message, sl.file_name(), sl.line()
                );
            }
            }
        }
    }

    constexpr auto exitOnInternalError(
        StrV message, std::source_location const sl = std::source_location::current()
    ) -> void {
        if constexpr (config::debugging) {
            log(ELogLevel::Fatal, std::format(
                    "Exit due to an internal error occurred in {}: {}",
                    sl.function_name(), message), sl);
        }
        std::exit(EXIT_FAILURE);
    }

    class ScopeTimer {
    public:
        explicit constexpr ScopeTimer(Fn<void(i64)>&& onDestroyed)
            : m_start{std::chrono::high_resolution_clock::now()},
              m_onDestroyed{onDestroyed} {}

        constexpr ~ScopeTimer() noexcept {
            m_onDestroyed(std::chrono::duration_cast<std::chrono::nanoseconds>(
                std::chrono::high_resolution_clock::now() - m_start
            ).count());
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
        Fn<void(i64)> m_onDestroyed;
    };

    class ScopeReporter {
    public:
        explicit constexpr ScopeReporter(
            std::source_location const sl = std::source_location::current()
        )
            : m_timer{
                [=](i64 const duration) {
                    log(ELogLevel::Info, std::format(
                            "Exit {}. Time elapsed: {} ns",
                            sl.function_name(),
                            intStringWithThousandsSep(duration)), sl);
                }
            } {
            log(ELogLevel::Info,
                std::format("Enter {}", sl.function_name()), sl);
        }

        ~ScopeReporter() noexcept {}

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
