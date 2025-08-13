add_library(tlc_static SHARED)
add_library(tlc::static ALIAS tlc_static)
target_sources(
    tlc_static
    PUBLIC FILE_SET CXX_MODULES FILES
    static.cppm
    error_collector.cpp

    post_parsing/post_parsing.cppm
    post_parsing/tuple_normalizer.cpp
)
target_link_libraries(
    tlc_static
    PUBLIC tlc::core tlc::token tlc::syntax
)