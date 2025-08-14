add_library(tlc_parse SHARED)
add_library(tlc::parse ALIAS tlc_parse)
target_sources(
    tlc_parse
    PRIVATE
    parse.hpp parse.cpp
    context.hpp context.cpp
    token_stream.hpp
    location_tracker.hpp location_tracker.cpp
    parse_error.hpp parse_error.cpp
    pretty_printer.hpp pretty_printer.cpp
    ast_printer.hpp ast_printer.cpp
    handle_expr.cpp
    handle_type.cpp
    handle_stmt.cpp
    handle_decl.cpp
    handle_enum_def.cpp
    handle_flag_def.cpp
    handle_function_def.cpp
    handle_trait_def.cpp
    handle_type_def.cpp
    handle_global_decl.cpp
)
target_link_libraries(
    tlc_parse
    PUBLIC tlc::core tlc::token tlc::syntax
)