add_library(ToyLang_Core STATIC)
add_library(
    ${ToyLang_PROJECT_NAME}::Core
    ALIAS ToyLang_Core
)

target_sources(
    ToyLang_Core
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Core.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Exceptions.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Platforms.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Logger.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Types.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Concepts.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileReader.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileReader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Utility.hpp
)
target_include_directories(ToyLang_Core INTERFACE ${PROJECT_SOURCE_DIR}/Source)
target_link_libraries(
    ToyLang_Core
    PUBLIC spdlog::spdlog_header_only
    #    INTERFACE ${ToyLang_PROJECT_NAME}::CFlags
)