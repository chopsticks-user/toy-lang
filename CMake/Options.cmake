include(GNUInstallDirs)

set(ToyLang_PROJECT_NAME ${PROJECT_NAME})

set(ToyLang_TARGET_NAME ${PROJECT_NAME})
option(ToyLang_BUILD_EXAMPLES "Build examples" ON)
option(ToyLang_BUILD_TESTS "Build tests" ON)

set(ToyLang_AS_SUBPROJECT OFF)
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  set(ToyLang_AS_SUBPROJECT ON)
endif ()

set(ToyLang_LLVM_VERSION 17)