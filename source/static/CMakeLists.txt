add_library(tlc_static SHARED)
add_library(tlc::static ALIAS tlc_static)
target_sources(
    tlc_static
    PRIVATE
    static.hpp
    error_collector.hpp error_collector.cpp
)
target_link_libraries(
    tlc_static
    PUBLIC tlc::core tlc::token tlc::syntax
)