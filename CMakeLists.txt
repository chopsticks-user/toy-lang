cmake_minimum_required(VERSION 3.15)
project(tlc VERSION 0.0 LANGUAGES C CXX)

include(cmake/Options.cmake)
include(cmake/CFlags.cmake)
include(FetchContent)

add_subdirectory(source)

if (${TLC_BUILD_TESTS})
  message(STATUS "Enable building tests")
  enable_testing()
  add_subdirectory(test)
endif ()

if (${TLC_BUILD_EXAMPLES})
  message(STATUS "Enable building examples")
  add_subdirectory(examples)
endif ()
