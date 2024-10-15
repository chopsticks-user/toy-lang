#ifndef  TOYLANG_SYNTAX_STATEMENTS_HPP
#define  TOYLANG_SYNTAX_STATEMENTS_HPP

#include "Base.hpp"
#include "Expressions.hpp"

#include "Core/Core.hpp"

namespace tl::syntax {
  class BlockStatement final : public Node {
  public:
    explicit BlockStatement(std::vector<VNode> statements);

  private:
  };

  class IdentifierDeclStatement final : public Node {
  public:
    IdentifierDeclStatement(std::vector<VNode> fragments, std::string mut);

    auto mutibility() -> const std::string & {
      return m_mutibility;
    }

  private:
    std::string m_mutibility;
  };

  class ModuleStatement {
  };

  class ImportStatement {
  };
}

#endif // TOYLANG_SYNTAX_STATEMENTS_HPP
