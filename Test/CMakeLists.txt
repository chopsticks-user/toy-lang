# Catch2
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

add_subdirectory(Utility)
add_subdirectory(Unit)
add_subdirectory(Performance)
add_subdirectory(E2E)
