add_executable(Builder)
add_executable(
    ${ToyLang_PROJECT_NAME}::Builder
    ALIAS Builder
)

target_sources(
    Builder
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Builder.cpp
)
#target_include_directories()
target_link_libraries(
    Builder
    PRIVATE
    spdlog::spdlog_header_only
    INTERFACE ${ToyLang_PROJECT_NAME}::CFlags
)