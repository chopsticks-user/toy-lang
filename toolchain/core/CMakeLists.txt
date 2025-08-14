# For the <execution> header
find_package(TBB REQUIRED COMPONENTS tbb)

if (${TLC_BUILD_EXAMPLES})
  add_compile_definitions(TLC_CONFIG_BUILD_EXAMPLES)
endif ()

if (${TLC_BUILD_TESTS})
  add_compile_definitions(TLC_CONFIG_BUILD_TESTS)
endif ()

add_library(tlc_core SHARED)
add_library(tlc::core ALIAS tlc_core)
target_sources(
    tlc_core
    PUBLIC FILE_SET CXX_MODULES FILES
    core.cppm
    core.fwd.cpp
    platform.cpp
    utility.cpp
    range.cpp
    exception.cpp
    concept.cpp
    visitor.cpp
    singleton.cpp
    mixin.cpp
)
target_precompile_headers(
    tlc_core
    PUBLIC
    core.pch.hpp
)
target_include_directories(
    tlc_core
    INTERFACE ${PROJECT_SOURCE_DIR}/toolchain
)
target_link_libraries(
    tlc_core
    INTERFACE tlc::cflags tbb
)