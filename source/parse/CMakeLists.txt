add_library(tlc_parse SHARED)
add_library(tlc::parse ALIAS tlc_parse)
target_sources(
    tlc_parse
    PRIVATE
    parse.hpp parse.cpp
    context.hpp context.cpp
    token_stream.hpp token_stream.cpp
    combinators.hpp combinators.cpp
    panic.hpp panic.cpp
    pretty_print.hpp pretty_print.cpp
    parse_enum.cpp
    parse_flag.cpp
    parse_function.cpp
    parse_module.cpp
    parse_stmt.cpp
    parse_trait.cpp
    parse_type.cpp
    parse_type_expr.cpp
    parse_value_expr.cpp
)
target_link_libraries(
    tlc_parse
    PUBLIC tlc::core
    PRIVATE tlc::token tlc::syntax
)