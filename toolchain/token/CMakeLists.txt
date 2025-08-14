add_library(tlc_token SHARED)
add_library(tlc::token ALIAS tlc_token)
target_sources(
    tlc_token PRIVATE
    token.hpp
    token_impl.hpp token_impl.cpp
    lexeme.hpp lexeme.cpp
    tokenized_buffer.hpp tokenized_buffer.cpp
)
target_link_libraries(
    tlc_token
    PUBLIC tlc::core
)