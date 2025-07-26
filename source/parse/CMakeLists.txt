add_library(tlc_parse SHARED)
add_library(tlc::parse ALIAS tlc_parse)
target_sources(
    tlc_parse
    PRIVATE
    parse.hpp parse.cpp
    context.hpp context.cpp
    token_stream.hpp token_stream.cpp
    combinator.hpp
    panic.hpp panic.cpp
    handle_expr.cpp
    handle_type.cpp
    handle_stmt.cpp
    handle_stmt_level_decl.cpp
    pretty_printer.hpp pretty_printer.cpp
    ast_printer.hpp ast_printer.cpp
)
target_link_libraries(
    tlc_parse
    PUBLIC tlc::core tlc::token tlc::syntax
)