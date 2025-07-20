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
    pretty_print.hpp pretty_print.cpp
    handle_enum.cpp
    handle_flag.cpp
    handle_function.cpp
    handle_module.cpp
    handle_stmt.cpp
    handle_trait.cpp
    handle_type.cpp
    handle_type_expr.cpp
    handle_value_expr.cpp
)
target_link_libraries(
    tlc_parse
    PUBLIC tlc::core tlc::token tlc::syntax
)