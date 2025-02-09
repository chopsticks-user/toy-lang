add_library(tlc_token SHARED)
add_library(tlc::token ALIAS tlc_token)
target_sources(
    tlc_token PRIVATE
    token.hpp token.cpp
)
target_link_libraries(
    tlc_token
    PUBLIC tlc::core
)