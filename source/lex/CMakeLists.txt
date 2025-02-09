add_library(tlc_lex SHARED)
add_library(tlc::lex ALIAS tlc_lex)
target_sources(
    tlc_lex PRIVATE
    lex.hpp lex.cpp context.hpp context.cpp stream.hpp stream.cpp
)
target_link_libraries(
    tlc_lex
    PUBLIC tlc::core
    PRIVATE tlc::token
)