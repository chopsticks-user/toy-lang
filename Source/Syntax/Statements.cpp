#include "Statements.hpp"
#include "Concrete.hpp"

namespace tl::syntax {
  BlockStatement::BlockStatement(std::vector<VNode> statements)
    : Node(std::move(statements)) {
  }

  IdentifierDeclStatement::IdentifierDeclStatement(
    std::vector<VNode> fragments, std::string mut
  ) : Node(std::move(fragments)), m_mutibility(std::move(mut)) {
  }
}
