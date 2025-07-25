add_library(tlc_lex SHARED)
add_library(tlc::lex ALIAS tlc_lex)
target_sources(
    tlc_lex PRIVATE
    lex.hpp lex.cpp
    text_stream.hpp text_stream.cpp
    util.hpp
    lex_comment.cpp
    lex_identifier.cpp
    lex_numeric.cpp
    lex_symbol.cpp
)
target_link_libraries(
    tlc_lex
    PUBLIC tlc::core tlc::token
)