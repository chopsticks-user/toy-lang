cmake_minimum_required(VERSION 3.15)
project(tlc VERSION 0.0 LANGUAGES C CXX)

include(FetchContent)
include(cmake/CFlags.cmake)
include(cmake/Options.cmake)

configure_file(
    ${CMAKE_SOURCE_DIR}/source/core/config.in.hpp
    ${CMAKE_SOURCE_DIR}/source/core/config.hpp
    #    @ONLY
)

add_subdirectory(source)

if (${TLC_CONFIG_BUILD_TESTS})
  message(STATUS "Enable building tests")
  enable_testing()
  add_subdirectory(test)
endif ()

if (${TLC_CONFIG_BUILD_EXAMPLES})
  message(STATUS "Enable building examples")
  add_subdirectory(examples)
endif ()
