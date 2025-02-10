add_library(tlc_syntax SHARED)
add_library(tlc::syntax ALIAS tlc_syntax)
target_sources(
    tlc_syntax PRIVATE
    syntax.hpp ast_base.hpp ast_base.cpp ast_visitor.hpp forward.hpp
    definition_nodes.hpp definition_nodes.cpp expr_nodes.hpp expr_nodes.cpp
    stmt_nodes.hpp stmt_nodes.cpp
)
target_link_libraries(tlc_syntax PRIVATE tlc::core)