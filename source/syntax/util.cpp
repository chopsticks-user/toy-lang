#include "util.hpp"
#include "nodes.hpp"

namespace tlc::syntax {
  auto isEmptyAst(const Node &node) -> bool {
    return std::holds_alternative<std::monostate>(node);
  }
}
