# Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

add_compile_definitions(
    TLC_TEST_SAMPLE_DIR="../../samples"
)
add_compile_definitions(
    TLC_TMP_FILE_DIR="../../tmp"
)

add_subdirectory(unit)
add_subdirectory(performance)
add_subdirectory(integration)
