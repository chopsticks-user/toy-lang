# LLVM
find_package(LLVM ${LLVM_VERSION} REQUIRED CONFIG)
include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Nlohmann Json
#FetchContent_Declare(
#    nlohmann_json
#    GIT_REPOSITORY https://github.com/nlohmann/json.git
#    GIT_TAG v3.11.3
#)
#FetchContent_MakeAvailable(nlohmann_json)

add_subdirectory(core)
add_subdirectory(utility)
add_subdirectory(token)
add_subdirectory(syntax)
add_subdirectory(lex)
add_subdirectory(parse)
add_subdirectory(static)
add_subdirectory(backend)
add_subdirectory(driver)
