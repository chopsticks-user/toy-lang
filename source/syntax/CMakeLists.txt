add_library(tlc_syntax SHARED)
add_library(tlc::syntax ALIAS tlc_syntax)
target_sources(
  tlc_syntax PRIVATE
  syntax.hpp base.hpp base.cpp visitor.hpp forward.hpp nodes.hpp nodes.cpp
  util.hpp util.cpp
)
target_link_libraries(tlc_syntax PRIVATE tlc::core)