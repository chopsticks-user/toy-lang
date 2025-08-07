#ifndef TLC_CORE_CONFIG_HPP
#define TLC_CORE_CONFIG_HPP

#define TLC_CONFIG_BUILD_TESTS
#define TLC_CONFIG_BUILD_EXAMPLES
#define TLC_CONFIG_BUILD_DEBUG
/* #undef TLC_CONFIG_ENABLE_LOGGING */
#define TLC_CONFIG_VERSION_MAJOR 0
#define TLC_CONFIG_VERSION_MINOR 0
#define TLC_CONFIG_VERSION_LLVM 17

namespace tlc::config {
#ifdef TLC_CONFIG_BUILD_EXAMPLES
    constexpr auto buildExamples = true;
#elif
    constexpr auto buildExamples = false;
#endif

#ifdef TLC_CONFIG_BUILD_TESTS
    constexpr auto buildTests = true;
#else
    constexpr auto buildTests = false;
#endif

#if defined(TLC_CONFIG_BUILD_DEBUG) && defined(TLC_CONFIG_ENABLE_LOGGING)
    constexpr auto debugging = true;
#else
    constexpr auto debugging = false;
#endif

    constexpr auto versionMajor = TLC_CONFIG_VERSION_MAJOR;
    constexpr auto versionMinor = TLC_CONFIG_VERSION_MINOR;
    constexpr auto versionLLVM = TLC_CONFIG_VERSION_LLVM;
}

#endif // TLC_CORE_CONFIG_HPP
