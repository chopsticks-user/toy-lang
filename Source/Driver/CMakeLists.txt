add_executable(${ToyLang_PROJECT_NAME})

llvm_map_components_to_libnames(llvm_libs support core irreader)
target_sources(
    ${ToyLang_PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Driver.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ConfigParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ConfigParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandParser.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandParser.cpp
)
target_include_directories(
    ${ToyLang_PROJECT_NAME}
    PRIVATE ${LLVM_INCLUDE_DIRS}
)
target_link_libraries(
    ${ToyLang_PROJECT_NAME}
    PRIVATE
    ${llvm_libs}
    ${ToyLang_PROJECT_NAME}::Core
    ${ToyLang_PROJECT_NAME}::FrontEnd
    ${ToyLang_PROJECT_NAME}::Syntax
    nlohmann_json
)
set_target_properties(
    ${ToyLang_PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_NAME tlc
)