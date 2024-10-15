cmake_minimum_required(VERSION 3.15)
project(ToyLang VERSION 0.0.1 LANGUAGES C CXX)

include(CMake/Options.cmake)
include(CMake/CFlags.cmake)
include(FetchContent)

add_subdirectory(Source)

if (${ToyLang_BUILD_TESTS})
  message(STATUS "Enable building tests")
  enable_testing()
  add_subdirectory(Test)
endif ()

if (${ToyLang_BUILD_EXAMPLES})
  message(STATUS "Enable building examples")
  add_subdirectory(Examples)
endif ()
