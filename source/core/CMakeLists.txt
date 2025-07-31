# For the <execution> header
find_package(TBB REQUIRED COMPONENTS tbb)

if (${TLC_BUILD_EXAMPLES})
  add_compile_definitions(TLC_CONFIG_BUILD_EXAMPLES)
endif ()

if (${TLC_BUILD_TESTS})
  add_compile_definitions(TLC_CONFIG_BUILD_TESTS)
endif ()

add_library(tlc_core SHARED)
add_library(tlc::core ALIAS tlc_core)
target_sources(
    tlc_core PRIVATE
    core.hpp platform.hpp type.hpp log.hpp log.cpp file.hpp file.cpp
    exception.hpp constant.hpp concept.hpp visitor.hpp singleton.hpp
    utility.hpp range.hpp graph.hpp graph.cpp
)
target_include_directories(tlc_core INTERFACE ${PROJECT_SOURCE_DIR}/source)
target_link_libraries(
    tlc_core
    PUBLIC tlc::cflags tbb
)