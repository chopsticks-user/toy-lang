#llvm_map_components_to_libnames(llvm_libs support core irreader)

add_library(tlc_driver SHARED)
add_library(tlc::driver ALIAS tlc_driver)
target_sources(
    tlc_driver
    PUBLIC FILE_SET CXX_MODULES FILES
    driver.cppm
    command.cpp
    project.cpp
    driver_impl.cpp
)
target_include_directories(
    tlc_driver PRIVATE
    ${LLVM_INCLUDE_DIRS}
)
target_link_libraries(
    tlc_driver
    PUBLIC tlc::core
    #    PRIVATE ${llvm_libs}
)

add_executable(tlc_entry entry.cpp)
add_executable(tlc::entry ALIAS tlc_entry)
target_link_libraries(
    tlc_entry
    PUBLIC
    tlc::core tlc::token tlc::syntax
    tlc::lex tlc::driver tlc::static
)
set_target_properties(
    tlc_entry PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    OUTPUT_NAME tlc
)
