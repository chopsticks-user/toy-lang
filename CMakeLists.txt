cmake_minimum_required(VERSION 3.15)
project(ToyLang VERSION 0.0.1 LANGUAGES C CXX)

include(cmake/Options.cmake)
include(cmake/CFlags.cmake)
include(FetchContent)

add_subdirectory(source)

if (${ToyLang_BUILD_TESTS})
  message(STATUS "Enable building tests")
  enable_testing()
  add_subdirectory(test)
endif ()

if (${ToyLang_BUILD_EXAMPLES})
  message(STATUS "Enable building examples")
  add_subdirectory(examples)
endif ()
