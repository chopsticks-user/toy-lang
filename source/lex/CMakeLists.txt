add_library(tlc_lex SHARED)
add_library(tlc::lex ALIAS tlc_lex)
target_sources(
    tlc_lex
    PUBLIC FILE_SET CXX_MODULES FILES
    lex.cpp
    text_stream.cpp
    util.cpp
)
target_link_libraries(
    tlc_lex
    PUBLIC tlc::core tlc::token
)