add_library(tlc_parse SHARED)
add_library(tlc::parse ALIAS tlc_parse)
target_sources(
    tlc_parse
    PUBLIC FILE_SET CXX_MODULES FILES
    parse.cppm
    parse_impl.cpp
    context.cpp
    token_stream.cpp
    location_tracker.cpp
    parse_error.cpp
    pretty_printer.cpp
    ast_printer.cpp
    parse_unit.fwd.cpp
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
    PUBLIC tlc::core tlc::lex tlc::token tlc::syntax
)