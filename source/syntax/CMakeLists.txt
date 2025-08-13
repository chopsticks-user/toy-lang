add_library(tlc_syntax SHARED)
add_library(tlc::syntax ALIAS tlc_syntax)
target_sources(
    tlc_syntax
    PUBLIC FILE_SET CXX_MODULES FILES
    syntax.cpp
    visitor.cpp
    nodes.cpp
    util.cpp
)
target_link_libraries(
    tlc_syntax
    PUBLIC tlc::core tlc::token
)