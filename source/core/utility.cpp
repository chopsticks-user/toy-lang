#include "utility.hpp"

#include <print>
#include <chrono>
#include <locale>

namespace tlc {
    auto intStringWithThousandsSep(i64 value) -> Str {
        return std::format(
            std::locale{std::locale{""}, new ThousandsSep}, "{:L}", value
        );
    }

    auto log(
        ELogLevel const level, StrV message, std::source_location const sl
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

    auto exitOnInternalError(StrV const message, std::source_location const sl) -> void {
        if constexpr (config::debugging) {
            log(ELogLevel::Fatal, std::format(
                    "Exit due to an internal error occurred in {}: {}",
                    sl.function_name(), message), sl);
        }
        std::exit(EXIT_FAILURE);
    }

    ScopeTimer::ScopeTimer(Fn<void(i64)>&& onDestroyed)
        : m_start{std::chrono::high_resolution_clock::now()},
          m_onDestroyed{onDestroyed} {}

    ScopeTimer::~ScopeTimer() noexcept {
        m_onDestroyed(std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::high_resolution_clock::now() - m_start
        ).count());
    }

    ScopeReporter::ScopeReporter(std::source_location const sl)
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

    ScopeReporter::~ScopeReporter() noexcept {}
}
