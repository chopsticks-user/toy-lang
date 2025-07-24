add_library(tlc_syntax SHARED)
add_library(tlc::syntax ALIAS tlc_syntax)
target_sources(
    tlc_syntax PRIVATE
    syntax.hpp
    visitor.hpp
    forward.hpp
    base.hpp base.cpp
    nodes.hpp nodes.cpp
    util.hpp util.cpp
    pretty_printer.hpp pretty_printer.cpp
    ast_printer.hpp ast_printer.cpp
)
target_link_libraries(tlc_syntax PUBLIC tlc::core tlc::token)