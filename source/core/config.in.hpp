#ifndef TLC_CORE_CONFIG_HPP
#define TLC_CORE_CONFIG_HPP

#cmakedefine TLC_CONFIG_BUILD_TESTS
#cmakedefine TLC_CONFIG_BUILD_EXAMPLES
#cmakedefine TLC_CONFIG_BUILD_DEBUG
#define TLC_CONFIG_VERSION_MAJOR @tlc_VERSION_MAJOR@
#define TLC_CONFIG_VERSION_MINOR @tlc_VERSION_MINOR@
#define TLC_CONFIG_VERSION_LLVM @TLC_CONFIG_VERSION_LLVM@

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

#ifdef TLC_CONFIG_BUILD_DEBUG
    constexpr auto debugging = true;
#else
    constexpr auto debugging = false;
#endif

    constexpr auto versionMajor = TLC_CONFIG_VERSION_MAJOR;
    constexpr auto versionMinor = TLC_CONFIG_VERSION_MINOR;
    constexpr auto versionLLVM = TLC_CONFIG_VERSION_LLVM;
}

#endif // TLC_CORE_CONFIG_HPP
