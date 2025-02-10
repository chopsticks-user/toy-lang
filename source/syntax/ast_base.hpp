#ifndef  TLC_SYNTAX_AST_BASE_HPP
#define  TLC_SYNTAX_AST_BASE_HPP

#include "forward.hpp"

#include "core/core.hpp"

namespace tlc::syntax {
  class NodeBase {
  public:
    auto children() const noexcept -> Span<Node const>;

    auto view() const noexcept -> Span<Node const>;

    auto childAt(sz index) const -> Node const &;

    auto firstChild() const -> Node const &;

    auto lastChild() const -> Node const &;

    auto nChildren() const noexcept -> sz;

  protected:
    explicit NodeBase(std::vector<Node> children) noexcept;

    auto childAt(sz index) -> Node &;

    auto firstChild() -> Node &;

    auto lastChild() -> Node &;

  private:
    Vec<Node> m_children;
  };

  auto isEmptyAst(Node const &node) -> bool;

  template<IsASTNode TNode>
  auto astCast(Node const &node, Str const &filepath = "") -> TNode {
    if constexpr (std::same_as<TNode, Node>) {
      return node;
    }

    if (!std::holds_alternative<TNode>(node)) {
      throw filepath.empty()
              ? InternalException(filepath, "invalid AST-node cast")
              : InternalException("invalid AST-node cast");
    }
    return std::get<TNode>(node);
  }

  template<std::derived_from<NodeBase>... TNode>
  auto matchAstType(Node const &node) -> bool {
    return (std::holds_alternative<TNode>(node) || ...);
  }
}

#endif // TLC_SYNTAX_AST_BASE_HPP
