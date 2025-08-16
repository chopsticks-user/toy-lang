add_library(tlc_static SHARED)
add_library(tlc::static ALIAS tlc_static)
target_sources(
    tlc_static PRIVATE
    static.hpp static.cpp
    static_error.hpp static_error.cpp

    post_parsing/tuple_reducer.hpp
    post_parsing/tuple_diagnostic.hpp
)
target_link_libraries(
    tlc_static
    PUBLIC tlc::core tlc::token tlc::syntax
)