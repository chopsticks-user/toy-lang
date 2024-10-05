# LLVM
find_package(LLVM ${LLVM_VERSION} REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})

# SpdLog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.14.1
)
FetchContent_MakeAvailable(spdlog)

add_subdirectory(Core)
add_subdirectory(Lexer)
add_subdirectory(Parser)
add_subdirectory(Syntax)
add_subdirectory(Semantic)
add_subdirectory(CodeGen)
add_subdirectory(Optimization)
add_subdirectory(Builder)
