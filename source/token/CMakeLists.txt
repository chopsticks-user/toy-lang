add_library(tlc_token SHARED)
add_library(tlc::token ALIAS tlc_token)
target_sources(
    tlc_token
    PUBLIC FILE_SET CXX_MODULES FILES
    token.cpp
    lexeme.cpp
)
target_link_libraries(
    tlc_token
    PUBLIC tlc::core
)