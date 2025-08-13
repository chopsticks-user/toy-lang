cmake_minimum_required(VERSION 3.15)
project(tlc VERSION 0.0 LANGUAGES C CXX)

include(FetchContent)
include(cmake/CFlags.cmake)
include(cmake/Options.cmake)

configure_file(
    ${CMAKE_SOURCE_DIR}/toolchain/core/config.in.cpp
    ${CMAKE_SOURCE_DIR}/toolchain/core/config.cpp
)

add_subdirectory(third_party)
add_subdirectory(toolchain)

if (${TLC_CONFIG_BUILD_TESTS})
  message(STATUS "Enable building tests")
  enable_testing()
  add_subdirectory(test)
endif ()

if (${TLC_CONFIG_BUILD_EXAMPLES})
  message(STATUS "Enable building examples")
  add_subdirectory(examples)
endif ()
